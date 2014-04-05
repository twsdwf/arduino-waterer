#include "makeconfig.h"
#include "ui_makeconfig.h"
#include "data_structs.h"
#include <QSettings>
#include <QFileDialog>
#include <QProcess>
#include "globalstatedialog.h"

#include<QDebug>
#include <algorithm>
#include <vector>

MakeConfig::MakeConfig(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MakeConfig)
{
    ui->setupUi(this);
    ui->cbChipType->clear();
    ui->cbChipType->addItem("CD4067", 16);
    ui->cbChipType->addItem("HCF4051", 8);
    ui->cbChipType->setCurrentIndex(0);
    is_new = true;
    port = 0;
    this->modified = false;
    buf.resize(0);
    last_found = -1;
    this->state =stNone;
    this->pots = 0;
    this->scanner = NULL;
    this->ui->cmbDevice->setEnabled(false);
    this->ui->btnConnect->setEnabled(false);
    this->ui->btDetect->setEnabled(false);
    this->ui->btDetect2->setEnabled(false);
    this->ui->btnGetVals->setEnabled(false);
    this->ui->btSend->setEnabled(false);
    this->ui->btApplyDetected->setEnabled(false);
//   this->ui->btSave->setEnabled(false);
    QObject::connect(this->ui->mnuExit, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(this->ui->mnuSetup, SIGNAL(triggered()),this, SLOT(onSetup()));
    ui->statusBar->showMessage(QString::fromLocal8Bit("Для начала работы найдите устройства"));
}

MakeConfig::~MakeConfig()
{
    if (this->port) {
        this->port->close();
        this->port = NULL;
    }
    delete ui;
}
/*
void MakeConfig::setPort(QextSerialPort *_port)
{
    this->port = _port;
    if (this->port && port->isOpen()) {
        QObject::connect(this->port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
        qDebug()<<"SLOT readyRead connected";
    }
}*/

void MakeConfig::on_spnChips_valueChanged(int arg1)
{
    this->resizeTable();
}

void MakeConfig::resizeTable()
{

    int rows = this->ui->spnRows->value();

    if (modified && (rows < this->ui->tblConfig->rowCount())) {
        ui->statusBar->showMessage(QString::fromLocal8Bit("Нельзя уменьшить кол-во строк в несохранённой конфигурации"));
        return;
    }
    ui->tblConfig->setRowCount(rows);
    int cols = 12;
    this->ui->tblConfig->setColumnCount(cols);
    this->ui->tblConfig->clear();
    int cc = 0;
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Растение")));
    this->ui->tblConfig->setColumnWidth(cc++, 300);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Чип")));
    this->ui->tblConfig->setColumnWidth(cc++, 40);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Пин")));
    this->ui->tblConfig->setColumnWidth(cc++, 40);
    if (cols > 8) {
        this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Дозировщик")));
        this->ui->tblConfig->setColumnWidth(cc++, 90);
    }
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Чашка")));
    this->ui->tblConfig->setColumnWidth(cc++, 40);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Порция")));
    this->ui->tblConfig->setColumnWidth(cc++, 70);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Мин")));
    this->ui->tblConfig->setColumnWidth(cc++, 75);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Макс")));
    this->ui->tblConfig->setColumnWidth(cc++, 75);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Макс.дн.")));
    this->ui->tblConfig->setColumnWidth(cc++, 80);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Тек.пок.")));
    this->ui->tblConfig->setColumnWidth(cc++, 80);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Состояние")));
    this->ui->tblConfig->setColumnWidth(cc++, 80);
    this->ui->tblConfig->setHorizontalHeaderItem(cc, new QTableWidgetItem(QString::fromLocal8Bit("Полив/подсушка")));
    this->ui->tblConfig->setColumnWidth(cc, 80);

}

void MakeConfig::on_cbChipType_currentIndexChanged(int index)
{
    this->resizeTable();
}

void MakeConfig::on_btDetect_clicked()
{
    if (!port) {
        ui->statusBar->showMessage(QString::fromLocal8Bit("Нет соединения"));
        return;
    }
    last_found = -1;
    this->state = stDetectPinStep1;
    buf.clear();
    port->write("getall\r\n");
    ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск сенсора, шаг 1..."));
}

void MakeConfig::on_btDetect2_clicked()
{
    if (!port) {
        ui->statusBar->showMessage(QString::fromLocal8Bit("Не подключён"));
        return;
    }
    last_found = -1;
    buf.clear();
    port->flush();
    this->state = stDetectPinStep2;
    port->write("getall\r\n");
    //port->flush();
    ui->statusBar->showMessage(QString::fromLocal8Bit("Шаг 2:ждём ответ"));
}

void MakeConfig::on_tblConfig_itemChanged(QTableWidgetItem *item)
{
    this->modified = true;
}

void MakeConfig::onDataAvailable()
{
    QByteArray data =  port->readAll();
//    qDebug()<<"read: "<<data;
    buf.append(data);
qDebug()<<"state"<<state<<"buf="<<buf;
    if (state == stReadCurState) {
        ui->statusBar->showMessage(QString::fromLocal8Bit("Получение текущего состояния..."));
        if (!buf.contains(";;")) return;
        ui->statusBar->showMessage(QString::fromLocal8Bit("Дешифровка текущего состояния..."));
        QString s = QString(buf);
        QStringList lines = s.split("\n");
        for (int i=0;i< lines.length(); ++i) {
            if (lines.at(i).trimmed().length() < 5) {
                //qDebug()<<"skipped"<<lines.at(i);
                continue;
            }
            s = lines.at(i);
            QTextStream st(&s,  QIODevice::ReadOnly);
            int pin, chip;
            st>>pin;
            st>>chip;
            int r =0;
            --chip;
            //qDebug()<<"chip"<<chip<<" pin "<< pin;
            while (ui->tblConfig->item(r, 1) && ui->tblConfig->item(r, 2)) {
                if (ui->tblConfig->item(r, 2)->text().toInt() == pin && ui->tblConfig->item(r, 1)->text().toInt() == chip) {
                    QTableWidgetItem*item = new QTableWidgetItem;
                    int val;
                    st>>val;
                    item->setText(QString(QString::fromLocal8Bit("%1").arg(val)));
                    ui->tblConfig->setItem(r, 9, item);
                    item = new QTableWidgetItem;
                    st>>val;
                    item->setText(QString::fromLocal8Bit(val?"Поливаем":"Не поливаем"));
                    ui->tblConfig->setItem(r, 10, item);
                    item = new QTableWidgetItem;
                    st>>val;
                    item->setText(QString::fromLocal8Bit(val?"Подсушка":"Полив"));
                    ui->tblConfig->setItem(r, 11, item);
                    break;
                }
                ++r;
            }//while
            ui->statusBar->showMessage(QString::fromLocal8Bit("Текущее состояние получено"));
            state = stNone;
        }
    }
    if (state == stSendConfigLines) {
        //qDebug()<<buf.contains("OK");
        if (buf.contains("OK")) {
            buf.clear();
            ui->statusBar->showMessage(QString::fromLocal8Bit("Отправляем строку конфигурации №%1").arg(lsi+1));
            this->sendConfigLine(++lsi);
        }
        return;
    }else if (stSendSensorsCount == state) {
        //qDebug()<<buf.contains("SET");
        if (buf.contains(("SET"))) {
            state = stSendPlantNames;
            this->lsi  = 0;
            ui->statusBar->showMessage(QString::fromLocal8Bit("Начинаем отправку названий растений..."));
            this->port->write("\7");
            this->port->flush();
            sleep(1);
            this->sendPlantName(lsi);
        }
        return;
    } else if (state == stSendPlantNames) {
        //qDebug()<<buf.contains("OK");
        if (buf.contains("OK")) {
            buf.clear();
            ui->statusBar->showMessage(QString::fromLocal8Bit("Отправка названия из строки %1").arg(lsi+1));
            this->sendPlantName(++lsi);
        }
        return;
    }
    //QStringList lines = buf.split("\n");
    //first line -- cmd echo
    //qDebug()<<data<<data.contains(';');
    if (buf.contains(';')) {
        buf = buf.trimmed();
        //qDebug()<<buf;
        if (state == stDetectPinStep1) {
            test1.resize(0);
        } else if(state == stDetectPinStep2) {
            test2.resize(0);
        } else {
            ui->statusBar->showMessage(QString::fromLocal8Bit("Неправильный код состояния. Возможно, идёт отсылка/приём данных..."));
            data.clear();
            return;
        }
            QString str = QString(buf);
            QTextStream st(&str,  QIODevice::ReadOnly);
            int n;
            st>>n;
            //qDebug() << "n="<<n;
            for (int i = 0; i < n; ++i) {
                int val;
                st>>val;
                if (state == stDetectPinStep1) {
                    test1.push_back(val);
                } else if (state ==stDetectPinStep2) {
                    test2.push_back(val);
                }
            }
            if (state == stDetectPinStep1) {
                ui->statusBar->showMessage(QString::fromLocal8Bit("Готов к шагу 2"));
            } else if (state == stDetectPinStep2) {
                qDebug()<<"state"<<state<<"buf="<<buf;
                if (test1.size()!=test2.size()) {
                    ui->statusBar->showMessage(QString::fromLocal8Bit("Ошибка: не совпадает размер данных"));
                    buf.clear();
                    //qDebug()<<"state=0, ln 216";
                    state = stNone;
                    return;
                }
                int max_df = 0, max_index=-1, _cdf;
                for (int i=0;i<test1.size(); ++i) {
                    _cdf = abs(test1.at(i)-test2.at(i));
                    if (_cdf > max_df) {
                        qDebug()<<"delta: "<<_cdf;
                        max_index = i;
                        max_df = _cdf;
                    }
                }
                if (max_index >=0 ) {
                    qDebug()<<"max_index "<<max_index;
                    int pins = ui->cbChipType->itemData((ui->cbChipType->currentIndex())).toInt();
                    qDebug()<<"max_index "<<max_index;
                    ui->statusBar->showMessage(QString::fromLocal8Bit("Чип %1 пин %2").arg(max_index/pins).arg(max_index%pins));
                    this->last_found = max_index;
                }
                state = stNone;
                //qDebug()<<"state=0, ln234";
            }
    }
}

void MakeConfig::on_btApplyDetected_clicked()
{
    //qDebug()<<ui->tblConfig->currentRow();
    QTableWidgetItem*item = ui->tblConfig->item(ui->tblConfig->currentRow(), 1);
    //qDebug()<<item;
    int pins = ui->cmbDevice->itemData((ui->cbChipType->currentIndex())).toInt();
    if (item) {
        item->setText(QString("%1").arg(last_found/pins));
    } else {
        item = new QTableWidgetItem;
        item->setText(QString("%1").arg(last_found/pins));
        ui->tblConfig->setItem(ui->tblConfig->currentRow(), 1, item);
    }

    item = ui->tblConfig->item(ui->tblConfig->currentRow(), 2);
    if (item) {
        item->setText(QString("%1").arg(last_found%pins));
    } else {
        item = new QTableWidgetItem;
        item->setText(QString("%1").arg(last_found%pins));
        ui->tblConfig->setItem(ui->tblConfig->currentRow(), 2, item);
    }
}

void MakeConfig::on_btSend_clicked()
{
    //qDebug()<<"state:"<<state;
    if (state == stSendConfigLines) {
        //this->port->write("\7");
        //this->port->flush();
        this->sendConfigLine(lsi);
        return;
        //this->state = stNone;
         //ui->statusBar->showMessage(QString::fromLocal8Bit("Неверный статус. Система занята."));
         //return;
    } else if (state == stSendPlantNames) {
        this->sendPlantName(lsi);
        return;
    }
    ui->statusBar->showMessage(QString::fromLocal8Bit("Начинаем отправлять конфигурацию..."));
    state = stSendConfigLines;
    lsi = 0;
    this->sendConfigLine(lsi);
}

void MakeConfig::sendConfigLine(int index)
{
    if (!this->port || !this->port->isOpen()) {
        ui->statusBar->showMessage(QString::fromLocal8Bit("Порт недоступен. Обрыв связи?"));
        return;
    }

    if ( (index >= this->ui->tblConfig->rowCount()) || !this->ui->tblConfig->item(index, 1 )
         || this->ui->tblConfig->item(index, 1 )->text().length() < 1) {
        state = stSendSensorsCount;
      //  qDebug()<<"SET SENSORS "<<index;
        this->port->write(QString("set sensors %1\n").arg(index).toLocal8Bit());
        return;
    }
    //qDebug()<<"LSI"<< index;
    QString line;
   /* if (!this->ui->tblConfig->item(index, 1 )) {
        qDebug()<<"end of send";
        return;
    }*/
/*
    qDebug()<<this->ui->tblConfig->item(index, 1)->text()<<//chip
            this->ui->tblConfig->item(index, 2)->text()<<//pin
            this->ui->tblConfig->item(index, 3)->text()<<//doser
            this->ui->tblConfig->item(index, 4)->text()<< // cup
            this->ui->tblConfig->item(index, 5)->text()<<//portion
            this->ui->tblConfig->item(index, 6)->text()<<//min sensor val
            this->ui->tblConfig->item(index, 7)->text()<<//max sensor val
            this->ui->tblConfig->item(index, 8)->text();//day max portion
    */
    int test = this->ui->tblConfig->item(index, 1)->text().toInt()+//chip
            this->ui->tblConfig->item(index, 2)->text().toInt()+//pin
            this->ui->tblConfig->item(index, 3)->text().toInt()+//doser
            this->ui->tblConfig->item(index, 4)->text().toInt()+ // cup
            this->ui->tblConfig->item(index, 5)->text().toInt()+//portion
            this->ui->tblConfig->item(index, 6)->text().toInt()+//min sensor val
            this->ui->tblConfig->item(index, 7)->text().toInt()+//max sensor val
            this->ui->tblConfig->item(index, 8)->text().toInt();
    if (test) {
        line = line.sprintf("set data %d %d %d %d %d %d %d %d %d\n",
                 index, //index
                 this->ui->tblConfig->item(index, 1)->text().toInt(),//chip
                 this->ui->tblConfig->item(index, 2)->text().toInt(),//pin
                 this->ui->tblConfig->item(index, 3)->text().toInt(),//doser
                 this->ui->tblConfig->item(index, 4)->text().toInt(), // cup
                 this->ui->tblConfig->item(index, 5)->text().toInt(),//portion
                 this->ui->tblConfig->item(index, 6)->text().toInt(),//min sensor val
                 this->ui->tblConfig->item(index, 7)->text().toInt(),//max sensor val
                 this->ui->tblConfig->item(index, 8)->text().toInt()//day max portion
                 );
  qDebug()<<"line"<<line;
        port->write(line.toLatin1());
    } else {

    }
}

void MakeConfig::sendPlantName(int index)
{
    if (index >= this->ui->tblConfig->rowCount() || this->ui->tblConfig->item(index, 0)->text().length() < 1) {
        ui->statusBar->showMessage(QString::fromLocal8Bit("Отправка конфигурации завершена"));
        state = stNone;
    //    qDebug()<<"state=0, ln334";
        return;
    }

    QString line(QString::fromLocal8Bit("set name %1 %2\r\n").arg(index,2,10,QChar('0')).arg(this->ui->tblConfig->item(index, 0)->text().mid(0, 15)));
    qDebug()<< line;
    port->write(line.toLatin1());
}


void MakeConfig::on_btSave_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.ini)"));
    QSettings settings(fileName/*QString("configs/sensors.ini")*/, QSettings::IniFormat);
    int i=0;
    qDebug()<<fileName;
    while (i < this->ui->tblConfig->rowCount() && ui->tblConfig->item(i, 0) && ui->tblConfig->item(i, 0)->text().length()) {
        QString key;
        qDebug()<<"row"<<i;
        key = key.sprintf("pot_%d/name", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,0)?this->ui->tblConfig->item(i,0)->text():"-");
        key = key.sprintf("pot_%d/chip", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,1)?this->ui->tblConfig->item(i,1)->text():"-");
        key = key.sprintf("pot_%d/pin", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,2)?this->ui->tblConfig->item(i, 2)->text():"-");
        key = key.sprintf("pot_%d/doser", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,3)?this->ui->tblConfig->item(i, 3)->text():"-");
        key = key.sprintf("pot_%d/bowl", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,4)?this->ui->tblConfig->item(i,4)->text():"-");
        key = key.sprintf("pot_%d/portion", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,5)?this->ui->tblConfig->item(i,5)->text():"-");
        key = key.sprintf("pot_%d/min", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,6)?this->ui->tblConfig->item(i, 6)->text():"-");
        key = key.sprintf("pot_%d/max", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,7)?this->ui->tblConfig->item(i, 7)->text():"-");
        key = key.sprintf("pot_%d/daymax", i);
        settings.setValue(key,  this->ui->tblConfig->item(i,8)?this->ui->tblConfig->item(i, 8)->text():"-");
        ++i;
    }
    settings.setValue("global/count", i);
    ui->statusBar->showMessage(QString::fromLocal8Bit("Конфигурация сохранена локально."));
}


void MakeConfig::on_btOpen_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.ini)"));
    QSettings settings(fileName/*QString("configs/sensors.ini")*/, QSettings::IniFormat);
    int i=-1, n=0;
    n = settings.value("global/count", 0).toInt()+20;
    this->ui->tblConfig->setRowCount(n);
    while ( ++i < n) {
        QString key, val;
        key = key.sprintf("pot_%d/name", i);
        val = settings.value(key, "").toString();
        QTableWidgetItem* item = ui->tblConfig->item(i, 0);

        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 0, item);
        }
        key = key.sprintf("pot_%d/chip", i);
        val = settings.value(key, "").toString();
        item = ui->tblConfig->item(i, 1);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 1, item);
        }
        key = key.sprintf("pot_%d/pin", i);
        val = settings.value(key, "").toString();
        item = ui->tblConfig->item(i, 2);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 2, item);
        }

        key = key.sprintf("pot_%d/doser", i);
        val = settings.value(key, "").toString();
        item = ui->tblConfig->item(i, 3);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 3, item);
        }

        key = key.sprintf("pot_%d/bowl", i);
        val = settings.value(key, "").toString();
        item = ui->tblConfig->item(i, 4);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 4, item);
        }

        key = key.sprintf("pot_%d/portion", i);
        val = settings.value(key, "").toString();
        item = ui->tblConfig->item(i, 5);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 5, item);
        }

        key = key.sprintf("pot_%d/min", i);
        val = settings.value(key, "").toString();
        item = ui->tblConfig->item(i, 6);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 6, item);
        }

        key = key.sprintf("pot_%d/max", i);
        val = settings.value(key, "").toString();
        item = ui->tblConfig->item(i, 7);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 7, item);
        }

        key = key.sprintf("pot_%d/daymax", i);
        val = settings.value(key, "").toString();
        item = ui->tblConfig->item(i, 8);
        if (item) {
            item->setText(val);
        } else {
            item = new QTableWidgetItem;
            item->setText(val);
            ui->tblConfig->setItem(i, 8, item);
        }
    }//while
}

void MakeConfig::on_btnGetVals_clicked()
{
    /*state = stReadCurValues;
    buf.clear();
    port->write("getall\r\n");
    */
    ui->statusBar->showMessage(QString::fromLocal8Bit("Получение текущего состояния поливалки..."));
    state = stReadCurState;
    buf.clear();
    port->write("\7atest\r\n");
}

void MakeConfig::on_btObserve_clicked()
{
    ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск устройств..."));
    this->scanner = new QProcess(this);
    QString program("/usr/bin/hcitool");
    QStringList arguments;
    arguments << "scan";
    QObject::connect(this->scanner, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onScanFinished(int,QProcess::ExitStatus)));
    this->scanner->setProcessChannelMode(QProcess::MergedChannels);
    this->scanner->start(program, arguments);
}

void MakeConfig::onScanFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
//    qDebug()<<"scan finished.";
    ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск завершён"));
    QString output(this->scanner->readAllStandardOutput());

    delete this->scanner;
    this->scanner = NULL;

    QStringList devs = output.split(QChar('\n'));
    if (devs.count() < 2 ) {
//        qDebug()<<"no devices found";
        ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск устройств не дал результатов"));
        return;
    }
    QRegExp re("\\s*([0-9a-z\\:]+)\\s*(\\w+)", Qt::CaseInsensitive);
    this->ui->cmbDevice->clear();
    for (int i = 1; i < devs.count(); ++i) {
        QString s = devs.at(i);
        if (re.indexIn(s) > -1) {
            this->ui->cmbDevice->addItem(re.cap(2).append(" [").append(re.cap(1)).append("]"), re.cap(1));
        }
    }//for i
    if (this->ui->cmbDevice->count() > 0) {
        this->ui->btnConnect->setEnabled(true);
        ui->cmbDevice->setEnabled(true);
    }
    ui->statusBar->showMessage(QString::fromLocal8Bit("Поиск завершён. Выберите устройство для связи."));
}

void MakeConfig::on_btnConnect_clicked()
{
    QString address = ui->cmbDevice->itemData((ui->cmbDevice->currentIndex())).toString();
    ui->statusBar->showMessage(QString::fromLocal8Bit("Соединяемся с %1").arg(address));
    if (this->port) {
        port->close();
        delete port;
        port = NULL;
    }

    QString cmd = QString("rfcomm unbind all; rfcomm bind rfcomm0 %1").arg(address);

    system(cmd.toLocal8Bit());


    PortSettings ps;
    ps.BaudRate = BAUD38400;
    ps.DataBits = DATA_8;
    ps.Parity = PAR_NONE;
    ps.StopBits = STOP_1;
    ps.FlowControl = FLOW_OFF;
    ps.Timeout_Millisec = 5;
    //qDebug()<<address;
    this->port = new QextSerialPort("/dev/rfcomm0", ps, QextSerialPort::EventDriven);
    if(!this->port->open(QIODevice::ReadWrite|QIODevice::Text)) {
        QMessageBox::critical(this, "Error", this->port->errorString());
        delete this->port;
        this->port= NULL;
        this->ui->cmbDevice->setEnabled(true);
        this->ui->btnConnect->setEnabled(true);
        this->ui->btDetect->setEnabled(false);
        this->ui->btDetect2->setEnabled(false);
        this->ui->btnGetVals->setEnabled(false);
        this->ui->btSend->setEnabled(false);
        this->ui->btApplyDetected->setEnabled(false);
        this->ui->btSave->setEnabled(false);
        return;
    } else {
        //this->ui->sbStatus->showMessage(QString::fromLocal8Bit("Подключено к %1(%2)").arg(ui->cmbDevice->currentText()).arg(ui->cmbDevice->itemData((ui->cmbDevice->currentIndex())).toString()));
        this->ui->cmbDevice->setEnabled(true);
        this->ui->btnConnect->setEnabled(true);
        this->ui->btDetect->setEnabled(true);
        this->ui->btDetect2->setEnabled(true);
        this->ui->btnGetVals->setEnabled(true);
        this->ui->btSend->setEnabled(true);
        this->ui->btApplyDetected->setEnabled(true);
        this->ui->btSave->setEnabled(true);
        ui->statusBar->showMessage(QString::fromLocal8Bit("Соединено с %1").arg(ui->cmbDevice->currentText()));
    }
    QObject::connect(this->port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
}

void MakeConfig::onSetup()
{
    GlobalStateDialog*dlg = new GlobalStateDialog(this);
    QObject::disconnect(this->port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    dlg->setPort(this->port);
    dlg->exec();
    QObject::connect(this->port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    delete dlg;
}

void MakeConfig::on_btnSort_clicked()
{
    int rows = -1;
    std::vector<PlantData> lines;
    PlantData v;
    while (++rows < ui->tblConfig->rowCount()) {
        if (ui->tblConfig->item(rows, 0) && ui->tblConfig->item(rows,0)->text().length() > 0) {
            v.chip  = this->ui->tblConfig->item(rows, 1)->text().toInt();//chip
            v.pin   = this->ui->tblConfig->item(rows, 2)->text().toInt();//pin
            v.doser = this->ui->tblConfig->item(rows, 3)->text().toInt();//doser
            v.bowl  = this->ui->tblConfig->item(rows, 4)->text().toInt(); // cup
            v.portion = this->ui->tblConfig->item(rows, 5)->text().toInt();//portion
            v.min = this->ui->tblConfig->item(rows, 6)->text().toInt();//min sensor val
            v.max = this->ui->tblConfig->item(rows, 7)->text().toInt();//max sensor val
            v.daymax = this->ui->tblConfig->item(rows, 8)->text().toInt(); //day max portion
            v.name = this->ui->tblConfig->item(rows, 0)->text();
            lines.push_back(v);
        } else {
            break;
        }
    }
    //ui->tblConfig->setRowCount(rows);
    std::sort(lines.begin(), lines.end());
    ui->tblConfig->setRowCount(0);
    ui->tblConfig->setRowCount(rows);
#define SETITEM(r, c, val) {item = ui->tblConfig->item(r, c);if (item) item->setText(val); else {item = new QTableWidgetItem;item->setText(val);ui->tblConfig->setItem(r, c, item);}}
#define INT2STR(val) (QString("%1").arg(val))
    QTableWidgetItem*item;
    for (int i = 0; i < lines.size(); ++i) {
        PlantData pd = lines.at(i);

        SETITEM(i, 0, pd.name);
        SETITEM(i, 1, INT2STR(pd.chip));
        SETITEM(i, 2, INT2STR(pd.pin));
        SETITEM(i, 3, INT2STR(pd.doser));
        SETITEM(i, 4, INT2STR(pd.bowl));
        SETITEM(i, 5, INT2STR(pd.portion));
        SETITEM(i, 6, INT2STR(pd.min));
        SETITEM(i, 7, INT2STR(pd.max));
        SETITEM(i, 8, INT2STR(pd.daymax));
        SETITEM(i, 9, QString(""));
        SETITEM(i, 10, QString(""));
        SETITEM(i, 11, QString(""));
    }//for i
    //ui->tblConfig->sortItems(4,Qt::SortOrder);
}
