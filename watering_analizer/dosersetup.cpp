#include "dosersetup.h"
#include "ui_dosersetup.h"
#include <QDebug>
dosersetup::dosersetup(QWidget *parent, QextSerialPort*port) :
    QDialog(parent),
    ui(new Ui::dosersetup)
{
    this->port = port;
    this->values.clear();
    this->values.push_back(0);
    this->values.push_back(17);
    this->values.push_back(29);
    this->values.push_back(41);
    this->values.push_back(51);
    //0,17,29,41,51,63,75,87,100,112,122,133,145,155,167,176
    ui->setupUi(this);
    this->updateState();
}

dosersetup::~dosersetup()
{
    this->values.clear();
    delete ui;
}

void dosersetup::on_btnDeleteLast_clicked()
{
    this->values.pop_back();
    this->updateState();
}

void dosersetup::on_btSavePos_clicked()
{
    this->values.push_back(this->ui->slAngle->value());
    this->updateState();
}


void dosersetup::updateState()
{
    this->ui->edResult->clear();
    QString str;
    for(QVector<int>::Iterator it = this->values.begin(); it != this->values.end(); ++it) {
        if(str.length() >0) {
            str.append(",");
        }
        str.append(QString("%1").arg(*it));
    }
    this->ui->edResult->setText(str);
}

void dosersetup::on_slAngle_sliderMoved(int position)
{
    this->ui->lbCurrentAngle->setText(QString("%1").arg(position));
    //QString cmd(QString::fromLocal8Bit("set wpos %1 %2\r\n").arg(this->values.size()+1).arg(this->ui->slAngle->value()));
    //this->port->write(cmd.toLatin1());
}

void dosersetup::on_slAngle_valueChanged(int value)
{

}

void dosersetup::on_btnTest_clicked()
{
    if ( !(this->port && this->port->isOpen())) {
        return;
    }

    if (this->ui->rdOneDoser->isChecked()) {
        /*QString cmd(QString::fromLocal8Bit("set wpos %1 %2\r\n").arg(this->values.size()+1).arg(this->ui->slAngle->value()));
        this->port->write(cmd.toLatin1());
        sleep(1);*/
        this->port->write("get pipi\r\n");
    } else {

    }
}

void dosersetup::on_btnMove_clicked()
{
    if (this->ui->rdOneDoser->isChecked()) {
        QString cmd(QString::fromLocal8Bit("set wpos %1 %2\r\n").arg(this->values.size()+1).arg(this->ui->slAngle->value()));
        this->port->write(cmd.toLatin1());
    }
}

void dosersetup::on_rdMultiDosers_toggled(bool checked)
{
    this->ui->rdDoser1->setEnabled(checked);
    this->ui->rdDoser2->setEnabled(checked);
}
