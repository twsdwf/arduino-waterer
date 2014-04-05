#if 0
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMessageBox>
#include <QProcess>
#include "com/qextserialport.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QextSerialPort*com;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_btnConnect_clicked();

    void on_btRecvConfig_clicked();
    void onDataAvailable();
    void on_btSendConfig_clicked();

    void on_btnFromFile_clicked();
    void on_mnuAnalize();
    void on_mnuDoserSetup();
    void on_mnuCfgEdit();
private:
    Ui::MainWindow *ui;
    int state;
    void initTable();
};

#endif // MAINWINDOW_H
#endif
