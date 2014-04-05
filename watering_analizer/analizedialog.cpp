#include "analizedialog.h"
#include "ui_analizedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <qwt_plot_grid.h>

AnalizeDialog::AnalizeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalizeDialog)
{
    QSettings settings(QString("configs/config.ini"), QSettings::IniFormat);
    ui->setupUi(this);
    this->ui->cmbDevice->clear();
    int n_devices = settings.value("main/ndevices", 0).toInt(); // settings.value() returns QVariant
    ui->cmbDevice->clear();
    for (int i = 0; i < n_devices; ++i) {
        QString name = settings.value(QString("device%1/name").arg(i), "").toString();
        QString addr = settings.value(QString("device%1/address").arg(i), "").toString();
        ui->cmbDevice->addItem(name, addr);
    }

    QObject::connect(this->ui->btnSelectFile, SIGNAL(clicked()),this, SLOT(on_btnSelectFileClick()));
    QObject::connect(this->ui->btnLoad, SIGNAL(clicked()), this, SLOT(on_btnLoadDataClick()));
    cvLight = new QwtPlotCurve(QString::fromLocal8Bit("Освещение"));
    cvOuterTemp = new QwtPlotCurve(QString::fromLocal8Bit("Температура снаружи"));
    cvInnerTemp = new QwtPlotCurve(QString::fromLocal8Bit("Температура внутри"));
}

AnalizeDialog::~AnalizeDialog()
{
    delete cvInnerTemp;
    delete cvLight;
    delete cvOuterTemp;
    delete ui;
}

void AnalizeDialog::on_btnSelectFileClick()
{
    QFileDialog::Options options;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QString::fromLocal8Bit("Выбор файла с данными"),
                                                    "~",
                                tr("Text Files (*.txt);;All Files (*)"),
                                &selectedFilter,
                                options);
    if (!fileName.isEmpty())
        ui->lblFilename->setText(fileName);
}

void AnalizeDialog::on_btnLoadDataClick()
{
    QFile data(this->ui->lblFilename->text());
    if (!data.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Can not open file");
        return;
    }

    this->m_data.clear();
int nw = 0, index;
QVector<QPointF> weather, inner, lights;

QVector<QVector<QPointF> > pots_data;
pots_data.resize(this->pots.size());
this->initRoomData();
QVector<QString> names;

QDateTime t0;

    while (!data.atEnd()) {
        QString line = data.readLine();
        QStringList row = line.split(';');
        packetWrapper pw;
        pw.time = QDateTime::fromString(row.at(0), "HH:mm:ss dd.MM.yyyy");
        if (t0.isNull()) {
            t0 = pw.time;
        }
        pw.type = row.at(1).toInt();
        QPointF pt;
        switch (pw.type) {
            case LOG_PACKET_TYPE_E:
                pw.pe.temp_outer = row.at(2).toInt();
                pw.pe.temp_inner = row.at(3).toInt();
                pw.pe.hum_inner = row.at(4).toInt();
                pw.pe.flags = row.at(5).toInt();
                pt.setX(/*pw.time.toMSecsSinceEpoch()*/nw);
                pt.setY(pw.pe.temp_outer);
                ++nw;
                if (weather.size() > 2) {
                    if ( (weather.last().y() == pw.pe.temp_outer) && (pw.pe.temp_outer == weather.at(weather.size()-2).y())) {
                        weather.last().setX(pt.rx());
                    } else {
                        weather.append(pt);
                    }
                } else {
                    weather.append(pt);
                }
                pt.setY(pw.pe.temp_inner);
                if (inner.size() > 2) {
                    if ( (inner.last().y() == pw.pe.temp_inner) && (pw.pe.temp_inner == inner.at(inner.size()-2).y())) {
                        inner.last().setX(pt.rx());
                    } else {
                        inner.append(pt);
                    }
                } else {
                    inner.append(pt);
                }
                pt.setY(pw.pe.flags);
                if (lights.size() > 2) {
                    if ( (lights.last().y() == pw.pe.flags) && (pw.pe.flags == lights.at(inner.size()-2).y())) {
                        lights.last().setX(pt.rx());
                    } else {
                        lights.append(pt);
                    }
                } else {
                    lights.append(pt);
                }
                break;
            case LOG_PACKET_TYPE_S:
                pw.ps.doser = row.at(2).toInt();
                pw.ps.bowl = row.at(3).toInt();
                pw.ps.value = row.at(4).toInt();
                pw.ps.flags = row.at(5).toInt();
                break;
            case LOG_PACKET_TYPE_W:
                pw.pw.doser = row.at(2).toInt();
                pw.pw.bowl = row.at(3).toInt();
                pw.pw.portion = row.at(4).toInt();
                index = this->bowl2index(pw.pw.doser, pw.pw.bowl);
                if (index > -1) {
                    if(names.size()<=index) {
                        names.resize(index+1);
                        names[index] = QString("d%1b%2").arg(pw.pw.doser).arg(pw.pw.bowl);
                    }
                    if(index >= pots_data.size()) {
                        pots_data.resize(index+1);
                        pots_data[index].resize(0);
                    }

                    int dt = t0.daysTo(pw.time);
                    if (dt >= pots_data.at(index).size()) {
                        pots_data[index].resize(dt+1);
                        pots_data[index][dt].setY(pw.pw.portion);
                        pots_data[index][dt].setX(dt);
                    } else {
                        pots_data[index][dt].setX(dt);
                        pots_data[index][dt].setY(pots_data[index][dt].y()+ pw.pw.portion);
                    }

                }
                break;
        }
        this->m_data.append(pw);
//        if(weather.size()>2048)weather.resize(2048);
    }

    for (int c=0;c < pots_data.size(); ++c) {
        for (int p=0;p < pots_data[c].size();++p) {
            pots_data[c][p].setX(p);
        }
    }

    cvInnerTemp->setSamples(inner);
    cvInnerTemp->setPen(QColor(Qt::red));
    cvInnerTemp->setStyle(QwtPlotCurve::Lines);
    cvOuterTemp->setSamples(weather);
    cvOuterTemp->setPen(QColor(Qt::darkBlue));
    cvOuterTemp->setStyle(QwtPlotCurve::Lines);
   // cvOuterTemp->setRenderHint(QwtPlotItem::RenderAntialiased);
    cvOuterTemp->attach(this->ui->plWeather);
    cvInnerTemp->attach(ui->plWeather);
    QwtPlotGrid*grid = new QwtPlotGrid();
    grid->enableX(1);
    grid->enableY(1);
    grid->enableYMin(1);
    grid->attach(ui->plWeather);
    QwtPlotGrid*grid2 = new QwtPlotGrid();
    grid2->enableX(1);
    grid2->enableY(1);
    grid2->enableYMin(1);
    grid2->attach(ui->plLight);

    cvLight->setSamples(lights);
    cvLight->setPen(QColor(Qt::yellow));
    cvLight->setStyle(QwtPlotCurve::Lines);
    cvLight->attach(ui->plLight);
    ui->plWeather->replot();
    ui->plLight->replot();
    QColor clr[17]={Qt::white, Qt::black,Qt::red,Qt::darkRed,Qt::green, Qt::darkGreen, Qt::blue,Qt::darkBlue,Qt::cyan,Qt::darkCyan,Qt::magenta,Qt::darkMagenta,Qt::yellow,Qt::darkYellow,Qt::gray,Qt::darkGray,Qt::lightGray};
    this->ui->lsCurves->clear();
    for (int i=0;i<pots_data.size();++i) {
        this->ui->lsCurves->addItem(names[i]);
        this->pots[i] = new QwtPlotCurve(names[i]);
        this->pots[i]->setSamples(pots_data[i]);
        this->pots[i]->setStyle(QwtPlotCurve::Lines);
        this->pots[i]->setPen(clr[i%17]);
        this->pots[i]->attach(this->ui->plWatering);
    }
    QwtPlotGrid*grid3 = new QwtPlotGrid();
    grid3->enableX(1);
    grid3->enableY(1);
    grid3->enableYMin(1);
    grid3->attach(ui->plWatering);

    this->ui->lsCurves->selectAll();
    ui->plWatering->replot();
}

void AnalizeDialog::initRoomData()
{
    QString address = ui->cmbDevice->itemData((ui->cmbDevice->currentIndex())).toString();
    QString file = address.replace(":","");
    QSettings settings(QString("configs/config_%1.ini").arg(file), QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    for (QList<QString>::Iterator it = groups.begin(); !(it == groups.end()); ++it) {
        int doser = settings.value(settings.value(QString("%1/field_4").arg(*it)).toString()).toInt();
        int bowl =  settings.value(settings.value(QString("%1/field_5").arg(*it)).toString()).toInt();
        indexmap.insert(doser*100+bowl, pots.size());
        pots.append(NULL);
    }
}

int AnalizeDialog::bowl2index(int doser, int bowl)
{
    QMap<int,int>::Iterator it = this->indexmap.find(doser*100+bowl);
    if (it!= indexmap.end()) {
        return it.value();
    } else {
        this->pots.resize(pots.size()+1);
        indexmap.insert(doser*100+bowl, pots.size()-1);
        return pots.size()-1;
    }
    return -1;
}

void AnalizeDialog::on_lsCurves_itemSelectionChanged()
{
    for (int i =0;i< this->ui->lsCurves->count();++i) {
        if (this->ui->lsCurves->item(i)->isSelected()) {
            this->pots[i]->show();
        } else {
            this->pots[i]->hide();
        }
    }
    this->ui->plWatering->replot();
}
