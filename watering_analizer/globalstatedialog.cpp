#include "globalstatedialog.h"
#include "ui_globalstatedialog.h"
#include <QDateTime>
#include <QDate>
#include <QTime>

#include<QDebug>

GlobalStateDialog::GlobalStateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GlobalStateDialog)
{
    ui->setupUi(this);
    this->port = NULL;
    this->state = gstNone;
    buf.clear();
}

GlobalStateDialog::~GlobalStateDialog()
{
    delete ui;
}

void GlobalStateDialog::setPort(QextSerialPort *port)
{
    this->port = port;
    if (this->port->isOpen()) {
        QObject::connect(this->port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
        this->state = gstReadState;
        buf.clear();
        port->write("getstate\r\n");
    }
}

void GlobalStateDialog::onDataAvailable()
{

    if (state == gstCalibrate) {
        buf.append(port->readAll());
        if (!buf.contains(';')) return;
        this->ui->edCalibrate->setText(buf);
        this->state = gstNone;
        buf.clear();
        return;
        //QTextStream stm;
        //stm.setString(&buf, QIODevice::ReadOnly);
    } else if (state == gstReadState) {
        buf.append(port->readAll());
        qDebug()<<buf;
        if (!buf.contains(";;")) return;
        QStringList ls = buf.split("\n");
        if (ls.count() < 3) {
            qDebug()<<"not enough data=(";
            state = gstNone;
            return;
        }
        QTextStream st;
        QString str = ls.at(0);
        st.setString(&str);
        int h,mi,s,d,mo,dow;
        st>>h;
        st>>mi;
        st>>s;
        st>>d;
        st>>mo;
        st>>dow;
        char*days[] = {"Пн","Вт","Ср","Чт","Пт","Сб","Вс"};
        ui->edTime->setText(QString::fromLocal8Bit("%1:%2:%3 %4.%5 %6").arg(h).arg(mi).arg(s).arg(d).arg(mo).arg(QString::fromLocal8Bit(days[ dow ])));
        str = ls.at(1);
        st.setString(&str);
        st>>h;
        ui->rdStart->setChecked(h==1);
        ui->rdStop->setChecked(h==0);
        st>>h;
        ui->lblNight->setText(QString::fromLocal8Bit(h==1?"Ночь":"День"));
        st>>h;
        ui->edNightWDFrom->setText(QString("%1").arg(h));
        st>>h;
        ui->edNightWDTo->setText(QString("%1").arg(h));
        st>>h;
        ui->edNightWeFrom->setText(QString("%1").arg(h));
        st>>h;
        ui->edNightWETo->setText(QString("%1").arg(h));
        ui->lsvErrors->clear();
        ui->lsvErrors->addItem(ls.at(2));
        str = ls.at(2);
        st.setString(&str);
        st.setIntegerBase(16);
        d = 0;
        qDebug()<<"parse errors str"<<str;
        int val = 0;
        do {
            qDebug()<<"base:"<<st.integerBase();
            st>>val;
            qDebug()<<val<< (val>>4);
            if (val == 0) break;
            if ( (val>>4) < 10) {
                switch (val&0x0F) {
                    case 0x01: ui->lsvErrors->addItem(QString::fromLocal8Bit("Насос #%1: нет данных от счётчика жидкости").arg(val>>4));
                        break;
                    case 0x00:
                        ui->lsvErrors->addItem(QString::fromLocal8Bit("Насос #%1 всё хорошо").arg(val>>4));
                        break;
                    default:
                        ui->lsvErrors->addItem(QString::fromLocal8Bit("Насос #%1: неизвестное состояние %2").arg(val>>4).arg(val&0x0F));
                        break;
                }
            } else {
                /*
                 *
    #define 	ERR_NUMPOS_MISMATCH		0x01
    #define 	ERR_MEMORY				0x02
    #define		ERR_OUT_OF_RANGE		0x03
    #define 	ERR_MISS				0x04
    #define 	ERR_UNRECOV_MISS		0x05
    */
                switch (val&0x0F) {
                    case 0x01: ui->lsvErrors->addItem(QString::fromLocal8Bit("Дозатор #%1: не совпадает кол-во чашек").arg((val>>4)-10));
                        break;
                    case 0x03:
                        ui->lsvErrors->addItem(QString::fromLocal8Bit("Дозатор #%1: ошибочный номер чашки").arg((val>>4)-10));
                        break;
                    case 0x04:
                        ui->lsvErrors->addItem(QString::fromLocal8Bit("Дозатор #%1: промах мимо чашки").arg((val>>4) -10));
                        break;
                    case 0x05:
                        ui->lsvErrors->addItem(QString::fromLocal8Bit("Дозатор #%1: неисправимая ошибка").arg((val>>4) -10));
                        break;
                    case 0:
                        ui->lsvErrors->addItem(QString::fromLocal8Bit("Дозатор #%1: всё хорошо").arg((val>>4) -10));
                        break;
                }
            }
        } while (val >= 0x10 && d++ < 4);

        buf.clear();
    } else if (state == gstSendCommand) {
        buf.append(port->readAll());
        if (buf.contains("\n")) {
            buf.clear();
            state = gstReadState;
            port->write("getstate\r\n");
        }
    }
}

void GlobalStateDialog::on_rdStart_toggled(bool checked)
{
    ui->rdStop->setChecked(!checked);
    if (checked && port) {
        state = gstSendCommand;
        port->write("start\r\nsave cfg\r\n");
    }
}

void GlobalStateDialog::on_rdStop_toggled(bool checked)
{
    ui->rdStart->setChecked(!checked);
    if (checked && port) {
        state = gstSendCommand;
        port->write("stop\r\nsave cfg\r\n");
    }
}

void GlobalStateDialog::on_btnSetNight_clicked()
{
    QString cmd(QString::fromLocal8Bit("set night %1 %2 %3 %4\r\nsave cfg\r\n")
                                        .arg(ui->edNightWDFrom->text())
                                        .arg(ui->edNightWDTo->text())
                                        .arg(ui->edNightWeFrom->text())
                                        .arg(ui->edNightWETo->text()
                                      )
                );
    if (port) {
        state=gstSendCommand;
        port->write(cmd.toLocal8Bit());
    }
}

void GlobalStateDialog::on_btnSetTime_clicked()
{
    QTextStream stm;
    QString cmd;
    stm.setString(&cmd);
    stm<<"set time ";
    QDateTime dt = QDateTime::currentDateTime();
    dt.addSecs(5);
    QTime time = dt.time();
    stm<<time.hour()<<':'<<time.minute()<<':'<<time.second()<<' ';
    QDate date = dt.date();
    stm<<date.day()<<'.'<<date.month()<<' '<<date.dayOfWeek()<<"\r\nsave cfg\r\n";
    qDebug()<<cmd;
    if (port) {
        state = gstSendCommand;
        port->write(cmd.toLocal8Bit());
    }
}

void GlobalStateDialog::on_btRefresh_clicked()
{
    if (port){
        state = gstReadState;
        port->write("getstate\r\n");
    }
}

void GlobalStateDialog::on_btnResetErrors_clicked()
{
    if (port){
        state = gstSendCommand;
        port->write("water reset\r\n");
    }
}

void GlobalStateDialog::on_btnCalibrate_clicked()
{
    ui->edCalibrate->clear();
    this->state = gstCalibrate;
    buf.clear();
    this->port->write("getall\r\n");
}
