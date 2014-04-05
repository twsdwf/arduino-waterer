#if 0
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QCryptographicHash>
#include "analizedialog.h"
#include "dosersetup.h"
#include "makeconfig.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings(QString("configs/config.ini"), QSettings::IniFormat);
  /*  settings.setValue("main/ndevices", 3);
    settings.setValue("device0/name", "my room");
    settings.setValue("device0/address", "00:11:11:25:08:63");
    settings.setValue("device1/name", "guest room [right+center]");
    settings.setValue("device1/address", "00:12:11:24:96:70");
    settings.setValue("device2/name", "guest room[left]");
    settings.setValue("device2/address", "00:12:12:04:42:68");
*/
    int n_devices = settings.value("main/ndevices", 0).toInt(); // settings.value() returns QVariant
    ui->cmbDevice->clear();
    for (int i = 0; i < n_devices; ++i) {
        QString name = settings.value(QString("device%1/name").arg(i), "").toString();
        QString addr = settings.value(QString("device%1/address").arg(i), "").toString();
        ui->cmbDevice->addItem(name, addr);
    }
    ui->cmbDevice->addItem("debug", "/dev/rfcomm0");
    this->ui->sbStatus->showMessage(QString::fromLocal8Bit("Нет соединения"));
    this->com = NULL;
    this->state = 0;
    QObject::connect(this->ui->mnuAnalize, SIGNAL(triggered()), this, SLOT(on_mnuAnalize()));
    QObject::connect(this->ui->mnuDoserSetup, SIGNAL(triggered()), this, SLOT(on_mnuDoserSetup()));
    QObject::connect(this->ui->actCfgEdit, SIGNAL(triggered()), this, SLOT(on_mnuCfgEdit()));
}

MainWindow::~MainWindow()
{
    if (this->com) {
        this->com->close();
        this->com = NULL;
    }
    delete ui;
}

void MainWindow::on_btnConnect_clicked()
{
//    QMessageBox::information(this, QString("dbg"), ui->cmbDevice->itemData((ui->cmbDevice->currentIndex())).toString());
    QString address = ui->cmbDevice->itemData((ui->cmbDevice->currentIndex())).toString();

    QString cmd = QString("rfcomm unbind all; rfcomm bind rfcomm0 %1").arg(address);
    qDebug()<<cmd;
    system(cmd.toLocal8Bit());
    this->ui->sbStatus->showMessage(QString::fromLocal8Bit("Порт создан"));

    PortSettings ps;
    ps.BaudRate = BAUD38400;
    ps.DataBits = DATA_8;
    ps.Parity = PAR_NONE;
    ps.StopBits = STOP_1;
    ps.FlowControl = FLOW_OFF;
    ps.Timeout_Millisec = 5;
    qDebug()<<address;
    this->com = new QextSerialPort("/dev/rfcomm0", ps, QextSerialPort::EventDriven);
    if(!this->com->open(QIODevice::ReadWrite|QIODevice::Text)) {
        QMessageBox::critical(this,"Error",this->com->errorString());
        delete this->com;
        this->com = NULL;
        return;
    } else {
        this->ui->sbStatus->showMessage(QString::fromLocal8Bit("Подключено к %1(%2)").arg(ui->cmbDevice->currentText()).arg(ui->cmbDevice->itemData((ui->cmbDevice->currentIndex())).toString()));
        this->ui->btnFromFile->setEnabled(true);
        ui->btRecvConfig->setEnabled(true);
        ui->btSendConfig->setEnabled(true);
    }
    QObject::connect(this->com, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
}

void MainWindow::on_btRecvConfig_clicked()
{
    if (!this->com) return;
    this->state = 1;
    char*cmd = "dump cfg\r\n\r\n";
    this->com->write(cmd, strlen(cmd));
    this->initTable();
}

void MainWindow::onDataAvailable()
{
    static QString str;
    char buf[512] = {0};
    this->com->readLine(buf, sizeof(buf));
    str.append(buf);
    if (state == 0) return;
    else if (state == 1) {
        QString address = ui->cmbDevice->itemData((ui->cmbDevice->currentIndex())).toString();
        QString file = address.replace(":","");
        QSettings settings(QString("configs/config_%1.ini").arg(file), QSettings::IniFormat);

        while (!str.isEmpty() && str.contains("\n")) {
            QString substr = str.mid(0, str.indexOf("\n"));
            if (!substr.startsWith("set data")) {
                str.remove(0, str.indexOf("\n")+1);
                continue;
            }
            substr.replace("set data","");
            QTextStream ts(&substr, QIODevice::ReadOnly);
            int data, row = this->ui->tblConfig->rowCount();
            this->ui->tblConfig->setRowCount(this->ui->tblConfig->rowCount() + 1);
            --row;
               for(int i = 0; i < 9; ++i) {
                    ts>>data;
                    QTableWidgetItem* item = new QTableWidgetItem;
                    item->setText(QString("%1").arg(data));
                    item->setTextAlignment(Qt::AlignCenter);
                    this->ui->tblConfig->setItem(row, i+1, item);
               }
               QTableWidgetItem* item = new QTableWidgetItem;
               item->setText(settings.value(QString("chip_%1_pin_%2/field_0").arg(this->ui->tblConfig->item(row, 2)->text())
                                             .arg(this->ui->tblConfig->item(row, 3)->text()),
                      "").toString());
               item->setTextAlignment(Qt::AlignCenter);
               this->ui->tblConfig->setItem(row, 0, item);
           str.remove(0, str.indexOf("\n")+1);
        }
    }
}

void MainWindow::on_btSendConfig_clicked()
{
    QString address = ui->cmbDevice->itemData((ui->cmbDevice->currentIndex())).toString();
    QString file = address.replace(":","");
    QSettings settings(QString("configs/config_%1.ini").arg(file), QSettings::IniFormat);
    for(int row = 0; row < this->ui->tblConfig->rowCount(); ++row) {
        QString cmd("set data ");
        int n_args = 0;
        for(int col = 0; col < 10; ++col) {
            if(!this->ui->tblConfig->item(row, col)) continue;

            QString key = QString("chip_%1_pin_%2/field_%3").arg(this->ui->tblConfig->item(row, 2)->text())
                    .arg(this->ui->tblConfig->item(row, 3)->text()).arg(col);
            QVariant val(this->ui->tblConfig->item(row, col)->text());
            if (col) {
                cmd.append(val.toString()).append(" ");
                ++n_args;
            }
            settings.setValue(key, val);
        }
        qDebug()<<n_args<<cmd;
        if (n_args == 9) {
            cmd.append("\r\n");
            this->com->write(cmd.toLatin1());
            this->ui->sbStatus->showMessage(QString::fromLocal8Bit("Загрузка строки %1").arg(row));
        }
    }
}

void MainWindow::on_btnFromFile_clicked()
{
    QString address = ui->cmbDevice->itemData((ui->cmbDevice->currentIndex())).toString();
    QString file = address.replace(":","");
    QSettings settings(QString("configs/config_%1.ini").arg(file), QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    groups.sort();

    this->initTable();

    int index = 0;
    int row = this->ui->tblConfig->rowCount() - 1;

    for (QList<QString>::Iterator it = groups.begin(); !(it == groups.end()); ++it) {
            QTableWidgetItem* item = new QTableWidgetItem;
            item->setText(QString("%1").arg(settings.value(QString("%1/field_0").arg(*it)).toString()));
            item->setTextAlignment(Qt::AlignCenter);
            this->ui->tblConfig->setItem(row, 0, item);
           for(int i = 1; i <= 9; ++i) {
                QTableWidgetItem* item = new QTableWidgetItem;
                item->setText(QString("%1").arg(settings.value(QString("%1/field_%2").arg(*it).arg(i)).toInt(), 4,10, QChar(' ')));
                item->setTextAlignment(Qt::AlignCenter);
                this->ui->tblConfig->setItem(row, i, item);
           }
           ++row;
           this->ui->tblConfig->setRowCount(this->ui->tblConfig->rowCount() + 1);
    }
}

void MainWindow::on_mnuAnalize()
{
    AnalizeDialog*dlg = new AnalizeDialog(this);
    dlg->show();
}

void MainWindow::on_mnuDoserSetup()
{
    dosersetup*dlg=new dosersetup(this, this->com);
    QObject::disconnect(this->com, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    dlg->exec();
    QObject::connect(this->com, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
}

void MainWindow::on_mnuCfgEdit()
{
    MakeConfig*mc = new MakeConfig(this);
    QObject::disconnect(this->com, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    mc->setPort(com);
  //  mc->exec();
    QObject::connect(this->com, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
}

void MainWindow::initTable()
{
    this->ui->tblConfig->setColumnCount(10);
    this->ui->tblConfig->clear();
    this->ui->tblConfig->setHorizontalHeaderItem(0, new QTableWidgetItem(QString::fromLocal8Bit("Растение")));
    this->ui->tblConfig->setHorizontalHeaderItem(1, new QTableWidgetItem(QString::fromLocal8Bit("Индекс")));
    this->ui->tblConfig->setHorizontalHeaderItem(2, new QTableWidgetItem(QString::fromLocal8Bit("Чип")));
    this->ui->tblConfig->setHorizontalHeaderItem(3, new QTableWidgetItem(QString::fromLocal8Bit("Пин")));
    this->ui->tblConfig->setHorizontalHeaderItem(4, new QTableWidgetItem(QString::fromLocal8Bit("Дозировщик")));
    this->ui->tblConfig->setHorizontalHeaderItem(5, new QTableWidgetItem(QString::fromLocal8Bit("Чашка")));
    this->ui->tblConfig->setHorizontalHeaderItem(6, new QTableWidgetItem(QString::fromLocal8Bit("Порция")));
    this->ui->tblConfig->setHorizontalHeaderItem(7, new QTableWidgetItem(QString::fromLocal8Bit("Мин.зн.сенсора")));
    this->ui->tblConfig->setHorizontalHeaderItem(8, new QTableWidgetItem(QString::fromLocal8Bit("Макс.зн.сенсора")));
    this->ui->tblConfig->setHorizontalHeaderItem(9, new QTableWidgetItem(QString::fromLocal8Bit("Макс.дн.порция")));
    this->ui->tblConfig->setRowCount(1);
}

#endif
