#ifndef GLOBALSTATEDIALOG_H
#define GLOBALSTATEDIALOG_H

#include <QDialog>
#include "com/qextserialport.h"

namespace Ui {
class GlobalStateDialog;
}

typedef enum{
    gstNone,
    gstReadState,
    gstSendCommand,
    gstCalibrate
} enumGSDState;

class GlobalStateDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GlobalStateDialog(QWidget *parent = 0);
    ~GlobalStateDialog();
    void setPort(QextSerialPort*port);
private:
    Ui::GlobalStateDialog *ui;
    QextSerialPort*port;
    enumGSDState state;
    QString buf;
private slots:
    void onDataAvailable();
    void on_rdStart_toggled(bool checked);
    void on_rdStop_toggled(bool checked);
    void on_btnSetNight_clicked();
    void on_btnSetTime_clicked();
    void on_btRefresh_clicked();
    void on_btnResetErrors_clicked();
    void on_btnCalibrate_clicked();
};

#endif // GLOBALSTATEDIALOG_H
