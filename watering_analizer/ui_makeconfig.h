/********************************************************************************
** Form generated from reading UI file 'makeconfig.ui'
**
** Created: Sun Jan 5 20:13:51 2014
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAKECONFIG_H
#define UI_MAKECONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MakeConfig
{
public:
    QAction *mnuExit;
    QAction *mnuSetup;
    QAction *action_2;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btObserve;
    QComboBox *cmbDevice;
    QPushButton *btnConnect;
    QPushButton *btDetect;
    QPushButton *btDetect2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btApplyDetected;
    QPushButton *btSave;
    QPushButton *btOpen;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnSort;
    QPushButton *btSend;
    QPushButton *btnGetVals;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label;
    QSpinBox *spnRows;
    QLabel *label_2;
    QComboBox *cbChipType;
    QTableWidget *tblConfig;
    QMenuBar *menuBar;
    QMenu *menu;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MakeConfig)
    {
        if (MakeConfig->objectName().isEmpty())
            MakeConfig->setObjectName(QString::fromUtf8("MakeConfig"));
        MakeConfig->resize(951, 592);
        mnuExit = new QAction(MakeConfig);
        mnuExit->setObjectName(QString::fromUtf8("mnuExit"));
        mnuSetup = new QAction(MakeConfig);
        mnuSetup->setObjectName(QString::fromUtf8("mnuSetup"));
        action_2 = new QAction(MakeConfig);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        centralWidget = new QWidget(MakeConfig);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btObserve = new QPushButton(centralWidget);
        btObserve->setObjectName(QString::fromUtf8("btObserve"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/res/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        btObserve->setIcon(icon);

        horizontalLayout->addWidget(btObserve);

        cmbDevice = new QComboBox(centralWidget);
        cmbDevice->setObjectName(QString::fromUtf8("cmbDevice"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cmbDevice->sizePolicy().hasHeightForWidth());
        cmbDevice->setSizePolicy(sizePolicy);
        cmbDevice->setBaseSize(QSize(180, 0));

        horizontalLayout->addWidget(cmbDevice);

        btnConnect = new QPushButton(centralWidget);
        btnConnect->setObjectName(QString::fromUtf8("btnConnect"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/res/connect.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnConnect->setIcon(icon1);

        horizontalLayout->addWidget(btnConnect);

        btDetect = new QPushButton(centralWidget);
        btDetect->setObjectName(QString::fromUtf8("btDetect"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/res/find.png"), QSize(), QIcon::Normal, QIcon::Off);
        btDetect->setIcon(icon2);

        horizontalLayout->addWidget(btDetect);

        btDetect2 = new QPushButton(centralWidget);
        btDetect2->setObjectName(QString::fromUtf8("btDetect2"));
        btDetect2->setIcon(icon2);

        horizontalLayout->addWidget(btDetect2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        btApplyDetected = new QPushButton(centralWidget);
        btApplyDetected->setObjectName(QString::fromUtf8("btApplyDetected"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/res/apply.png"), QSize(), QIcon::Normal, QIcon::Off);
        btApplyDetected->setIcon(icon3);

        horizontalLayout->addWidget(btApplyDetected);

        btSave = new QPushButton(centralWidget);
        btSave->setObjectName(QString::fromUtf8("btSave"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/res/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        btSave->setIcon(icon4);

        horizontalLayout->addWidget(btSave);

        btOpen = new QPushButton(centralWidget);
        btOpen->setObjectName(QString::fromUtf8("btOpen"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/res/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        btOpen->setIcon(icon5);

        horizontalLayout->addWidget(btOpen);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btnSort = new QPushButton(centralWidget);
        btnSort->setObjectName(QString::fromUtf8("btnSort"));

        horizontalLayout->addWidget(btnSort);

        btSend = new QPushButton(centralWidget);
        btSend->setObjectName(QString::fromUtf8("btSend"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/res/transmit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btSend->setIcon(icon6);

        horizontalLayout->addWidget(btSend);

        btnGetVals = new QPushButton(centralWidget);
        btnGetVals->setObjectName(QString::fromUtf8("btnGetVals"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/res/download.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnGetVals->setIcon(icon7);

        horizontalLayout->addWidget(btnGetVals);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        spnRows = new QSpinBox(centralWidget);
        spnRows->setObjectName(QString::fromUtf8("spnRows"));
        spnRows->setValue(20);

        horizontalLayout->addWidget(spnRows);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        cbChipType = new QComboBox(centralWidget);
        cbChipType->setObjectName(QString::fromUtf8("cbChipType"));

        horizontalLayout->addWidget(cbChipType);


        verticalLayout->addLayout(horizontalLayout);

        tblConfig = new QTableWidget(centralWidget);
        tblConfig->setObjectName(QString::fromUtf8("tblConfig"));

        verticalLayout->addWidget(tblConfig);

        MakeConfig->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MakeConfig);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 951, 21));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        MakeConfig->setMenuBar(menuBar);
        statusBar = new QStatusBar(MakeConfig);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MakeConfig->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(mnuSetup);
        menu->addSeparator();
        menu->addAction(mnuExit);

        retranslateUi(MakeConfig);

        cbChipType->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MakeConfig);
    } // setupUi

    void retranslateUi(QMainWindow *MakeConfig)
    {
        MakeConfig->setWindowTitle(QApplication::translate("MakeConfig", "Waterer configurator", 0, QApplication::UnicodeUTF8));
        mnuExit->setText(QApplication::translate("MakeConfig", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        mnuSetup->setText(QApplication::translate("MakeConfig", "\320\223\320\273\320\276\320\261\320\260\320\273\321\214\320\275\321\213\320\265 \320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270...", 0, QApplication::UnicodeUTF8));
        action_2->setText(QApplication::translate("MakeConfig", "-", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btObserve->setToolTip(QApplication::translate("MakeConfig", "\320\237\320\276\320\270\321\201\320\272 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btObserve->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnConnect->setToolTip(QApplication::translate("MakeConfig", "\320\241\320\276\320\265\320\264\320\270\320\275\320\270\321\202\321\214\321\201\321\217", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btnConnect->setText(QString());
#ifndef QT_NO_TOOLTIP
        btDetect->setToolTip(QApplication::translate("MakeConfig", "\320\235\320\260\320\271\321\202\320\270 \321\201\320\265\320\275\321\201\320\276\321\200,\321\210 1", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btDetect->setText(QApplication::translate("MakeConfig", "\321\210\320\260\320\263 I", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btDetect2->setToolTip(QApplication::translate("MakeConfig", "\320\235\320\260\320\271\321\202\320\270 \321\201\320\265\320\275\321\201\320\276\321\200, \321\2102", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btDetect2->setText(QApplication::translate("MakeConfig", "\320\250\320\260\320\263 II", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btApplyDetected->setToolTip(QApplication::translate("MakeConfig", "\320\227\320\260\320\277\320\270\321\201\320\260\321\202\321\214 \321\201\320\265\320\275\321\201\320\276\321\200 \320\262 \321\202\320\265\320\272\321\203\321\211\321\203\321\216 \321\201\321\202\321\200\320\276\320\272\321\203", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btApplyDetected->setText(QString());
#ifndef QT_NO_TOOLTIP
        btSave->setToolTip(QApplication::translate("MakeConfig", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btSave->setText(QString());
#ifndef QT_NO_TOOLTIP
        btOpen->setToolTip(QApplication::translate("MakeConfig", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btOpen->setText(QString());
        btnSort->setText(QApplication::translate("MakeConfig", "Sort", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btSend->setToolTip(QApplication::translate("MakeConfig", "\320\236\321\202\320\277\321\200\320\260\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btSend->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnGetVals->setToolTip(QApplication::translate("MakeConfig", "\320\227\320\260\320\277\321\200\320\276\321\201\320\270\321\202\321\214 \321\202\320\265\320\272\321\203\321\211\320\265\320\265 \321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btnGetVals->setText(QString());
        label->setText(QApplication::translate("MakeConfig", "\320\232\320\276\320\273-\320\262\320\276 \321\201\321\202\321\200\320\276\320\272", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MakeConfig", "\321\202\320\270\320\277 \321\207\320\270\320\277\320\276\320\262", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("MakeConfig", "\320\244\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MakeConfig: public Ui_MakeConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAKECONFIG_H
