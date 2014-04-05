/********************************************************************************
** Form generated from reading UI file 'analizedialog.ui'
**
** Created: Thu Jan 2 20:02:37 2014
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANALIZEDIALOG_H
#define UI_ANALIZEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_AnalizeDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tabPrefs;
    QFormLayout *formLayout;
    QLabel *label;
    QComboBox *cmbDevice;
    QLabel *label_2;
    QPushButton *btnSelectFile;
    QPushButton *btnLoad;
    QLabel *lblFilename;
    QWidget *tabChart;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QwtPlot *plWeather;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox_2;
    QwtPlot *plLight;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_4;
    QListWidget *lsCurves;
    QwtPlot *plWatering;

    void setupUi(QDialog *AnalizeDialog)
    {
        if (AnalizeDialog->objectName().isEmpty())
            AnalizeDialog->setObjectName(QString::fromUtf8("AnalizeDialog"));
        AnalizeDialog->resize(632, 411);
        horizontalLayout = new QHBoxLayout(AnalizeDialog);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tabWidget = new QTabWidget(AnalizeDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabPrefs = new QWidget();
        tabPrefs->setObjectName(QString::fromUtf8("tabPrefs"));
        formLayout = new QFormLayout(tabPrefs);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(tabPrefs);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        cmbDevice = new QComboBox(tabPrefs);
        cmbDevice->setObjectName(QString::fromUtf8("cmbDevice"));

        formLayout->setWidget(0, QFormLayout::FieldRole, cmbDevice);

        label_2 = new QLabel(tabPrefs);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        btnSelectFile = new QPushButton(tabPrefs);
        btnSelectFile->setObjectName(QString::fromUtf8("btnSelectFile"));

        formLayout->setWidget(1, QFormLayout::FieldRole, btnSelectFile);

        btnLoad = new QPushButton(tabPrefs);
        btnLoad->setObjectName(QString::fromUtf8("btnLoad"));

        formLayout->setWidget(2, QFormLayout::FieldRole, btnLoad);

        lblFilename = new QLabel(tabPrefs);
        lblFilename->setObjectName(QString::fromUtf8("lblFilename"));

        formLayout->setWidget(2, QFormLayout::LabelRole, lblFilename);

        tabWidget->addTab(tabPrefs, QString());
        tabChart = new QWidget();
        tabChart->setObjectName(QString::fromUtf8("tabChart"));
        horizontalLayout_2 = new QHBoxLayout(tabChart);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox = new QGroupBox(tabChart);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));

        horizontalLayout_2->addWidget(groupBox);

        plWeather = new QwtPlot(tabChart);
        plWeather->setObjectName(QString::fromUtf8("plWeather"));

        horizontalLayout_2->addWidget(plWeather);

        tabWidget->addTab(tabChart, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_3 = new QHBoxLayout(tab);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));

        horizontalLayout_3->addWidget(groupBox_2);

        plLight = new QwtPlot(tab);
        plLight->setObjectName(QString::fromUtf8("plLight"));

        horizontalLayout_3->addWidget(plLight);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayout_4 = new QHBoxLayout(tab_2);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lsCurves = new QListWidget(tab_2);
        lsCurves->setObjectName(QString::fromUtf8("lsCurves"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lsCurves->sizePolicy().hasHeightForWidth());
        lsCurves->setSizePolicy(sizePolicy);
        lsCurves->setMaximumSize(QSize(150, 16777215));
        lsCurves->setSelectionMode(QAbstractItemView::MultiSelection);

        horizontalLayout_4->addWidget(lsCurves);

        plWatering = new QwtPlot(tab_2);
        plWatering->setObjectName(QString::fromUtf8("plWatering"));

        horizontalLayout_4->addWidget(plWatering);

        tabWidget->addTab(tab_2, QString());

        horizontalLayout->addWidget(tabWidget);


        retranslateUi(AnalizeDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AnalizeDialog);
    } // setupUi

    void retranslateUi(QDialog *AnalizeDialog)
    {
        AnalizeDialog->setWindowTitle(QApplication::translate("AnalizeDialog", "Dialog", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        tabWidget->setToolTip(QApplication::translate("AnalizeDialog", "<html><head/><body><p>\320\244\320\260\320\271\320\273 \320\264\320\260\320\275\320\275\321\213\321\205</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("AnalizeDialog", "\320\243\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\276", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AnalizeDialog", "\320\244\320\260\320\271\320\273 \320\264\320\260\320\275\320\275\321\213\321\205", 0, QApplication::UnicodeUTF8));
        btnSelectFile->setText(QApplication::translate("AnalizeDialog", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        btnLoad->setText(QApplication::translate("AnalizeDialog", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        lblFilename->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabPrefs), QApplication::translate("AnalizeDialog", "\320\244\320\260\320\271\320\273 \320\264\320\260\320\275\320\275\321\213\321\205", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("AnalizeDialog", "GroupBox", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabChart), QApplication::translate("AnalizeDialog", "\320\237\320\276\320\263\320\276\320\264\320\260 \321\201\320\275\320\260\321\200\321\203\320\266\320\270", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("AnalizeDialog", "GroupBox", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("AnalizeDialog", "\320\236\321\201\320\262\320\265\321\211\321\221\320\275\320\275\320\276\321\201\321\202\321\214", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("AnalizeDialog", "\320\237\320\276\320\273\320\270\320\262", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AnalizeDialog: public Ui_AnalizeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANALIZEDIALOG_H
