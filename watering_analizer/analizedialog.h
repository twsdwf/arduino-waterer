#ifndef ANALIZEDIALOG_H
#define ANALIZEDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QList>
#include "data_structs.h"
#include <qwt_plot_curve.h>

namespace Ui {
class AnalizeDialog;
}

class AnalizeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AnalizeDialog(QWidget *parent = 0);
    ~AnalizeDialog();
public slots:
    void on_btnSelectFileClick();
    void on_btnLoadDataClick();
private slots:
    void on_lsCurves_itemSelectionChanged();

private:
    Ui::AnalizeDialog *ui;
    QList<packetWrapper> m_data;
    QwtPlotCurve* cvLight, *cvOuterTemp, *cvInnerTemp;
    QVector<QwtPlotCurve*>pots;
    void initRoomData();
    int bowl2index(int, int);
    QMap<int,int> indexmap;
};

#endif // ANALIZEDIALOG_H
