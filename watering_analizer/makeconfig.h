#ifndef MAKECONFIG_H
#define MAKECONFIG_H

#include <QMainWindow>
//#include <QDialog>
#include "com/qextserialport.h"
#include <QMessageBox>
#include<QTableWidgetItem>
#include<QVector>
#include <QProcess>

namespace Ui {
class MakeConfig;
}

typedef enum StateEnum{
    stNone,
    stDetectPinStep1,
    stDetectPinStep2,
    stReadCurValues,
    stSendConfigLines,
    stSendPlantNames,
    stSendSensorsCount,
    stReadCurState
} StateEnum;

class MakeConfig : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MakeConfig(QWidget *parent = 0);
    ~MakeConfig();
private slots:
    void on_spnChips_valueChanged(int arg1);

    void on_cbChipType_currentIndexChanged(int index);

    void on_btDetect_clicked();

    void on_tblConfig_itemChanged(QTableWidgetItem *item);

    void onDataAvailable();

    void on_btApplyDetected_clicked();

    void on_btSend_clicked();
    void on_btDetect2_clicked();

    void on_btSave_clicked();

    void on_btOpen_clicked();

    void on_btnGetVals_clicked();

    void on_btObserve_clicked();
    void onScanFinished (int exitCode, QProcess::ExitStatus exitStatus);
    void on_btnConnect_clicked();
    void onSetup();
    void on_btnSort_clicked();

protected:
    void sendConfigLine(int index);
    void sendPlantName(int index);
private:
    Ui::MakeConfig *ui;
    bool is_new, modified;
    int last_found;
    void resizeTable();
    QextSerialPort*port;
    QString buf;
    QVector<int> test1, test2;
    StateEnum state;
    int lsi, pots;
    QProcess*scanner;
};

#endif // MAKECONFIG_H
