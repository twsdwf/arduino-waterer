#ifndef DOSERSETUP_H
#define DOSERSETUP_H

#include <QDialog>
#include <QVector>
#include "com/qextserialport.h"

namespace Ui {
class dosersetup;
}

class dosersetup : public QDialog
{
    Q_OBJECT
    
public:
    explicit dosersetup(QWidget *parent = 0, QextSerialPort*port=NULL);
    ~dosersetup();
private slots:
    void on_btnDeleteLast_clicked();

    void on_btSavePos_clicked();

    void on_slAngle_sliderMoved(int position);

    void on_slAngle_valueChanged(int value);

    void on_btnTest_clicked();

    void on_btnMove_clicked();

    void on_rdMultiDosers_toggled(bool checked);

private:
    QextSerialPort*port;
    Ui::dosersetup *ui;
    QVector<int> values;
    void updateState();
};

#endif // DOSERSETUP_H
