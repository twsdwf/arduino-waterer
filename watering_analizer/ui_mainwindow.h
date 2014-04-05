/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Jan 1 19:17:07 2014
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

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
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *mnuAnalize;
    QAction *action_2;
    QAction *mnuDoserSetup;
    QAction *actCfgEdit;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *cmbDevice;
    QPushButton *btnConnect;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btRecvConfig;
    QPushButton *btSendConfig;
    QPushButton *btnFromFile;
    QSpacerItem *horizontalSpacer;
    QTableWidget *tblConfig;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *sbStatus;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(765, 447);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        mnuAnalize = new QAction(MainWindow);
        mnuAnalize->setObjectName(QString::fromUtf8("mnuAnalize"));
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        mnuDoserSetup = new QAction(MainWindow);
        mnuDoserSetup->setObjectName(QString::fromUtf8("mnuDoserSetup"));
        actCfgEdit = new QAction(MainWindow);
        actCfgEdit->setObjectName(QString::fromUtf8("actCfgEdit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        cmbDevice = new QComboBox(centralWidget);
        cmbDevice->setObjectName(QString::fromUtf8("cmbDevice"));

        horizontalLayout->addWidget(cmbDevice);

        btnConnect = new QPushButton(centralWidget);
        btnConnect->setObjectName(QString::fromUtf8("btnConnect"));

        horizontalLayout->addWidget(btnConnect);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btRecvConfig = new QPushButton(centralWidget);
        btRecvConfig->setObjectName(QString::fromUtf8("btRecvConfig"));
        btRecvConfig->setEnabled(false);

        horizontalLayout->addWidget(btRecvConfig);

        btSendConfig = new QPushButton(centralWidget);
        btSendConfig->setObjectName(QString::fromUtf8("btSendConfig"));
        btSendConfig->setEnabled(false);

        horizontalLayout->addWidget(btSendConfig);

        btnFromFile = new QPushButton(centralWidget);
        btnFromFile->setObjectName(QString::fromUtf8("btnFromFile"));
        btnFromFile->setEnabled(false);

        horizontalLayout->addWidget(btnFromFile);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        tblConfig = new QTableWidget(centralWidget);
        tblConfig->setObjectName(QString::fromUtf8("tblConfig"));
        tblConfig->setAlternatingRowColors(true);
        tblConfig->setSortingEnabled(true);

        verticalLayout->addWidget(tblConfig);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 765, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        sbStatus = new QStatusBar(MainWindow);
        sbStatus->setObjectName(QString::fromUtf8("sbStatus"));
        MainWindow->setStatusBar(sbStatus);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(mnuAnalize);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuFile->addAction(mnuDoserSetup);
        menuFile->addAction(actCfgEdit);

        retranslateUi(MainWindow);
        QObject::connect(actionExit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        mnuAnalize->setText(QApplication::translate("MainWindow", "\320\237\321\200\320\276\320\260\320\275\320\260\320\273\320\270\320\267\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\273\320\276\320\263", 0, QApplication::UnicodeUTF8));
        action_2->setText(QApplication::translate("MainWindow", "-", 0, QApplication::UnicodeUTF8));
        mnuDoserSetup->setText(QApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 \320\264\320\276\320\267\320\260\321\202\320\276\321\200\320\276\320\262", 0, QApplication::UnicodeUTF8));
        actCfgEdit->setText(QApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214 \320\272\320\276\320\275\321\204\320\270\320\263", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "\320\243\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\276", 0, QApplication::UnicodeUTF8));
        btnConnect->setText(QApplication::translate("MainWindow", "\320\241\320\276\320\265\320\264\320\270\320\275\320\270\321\202\321\214\321\201\321\217", 0, QApplication::UnicodeUTF8));
        btRecvConfig->setText(QApplication::translate("MainWindow", "\320\237\320\276\320\273\321\203\321\207\320\270\321\202\321\214 \320\272\320\276\320\275\321\204\320\270\320\263", 0, QApplication::UnicodeUTF8));
        btSendConfig->setText(QApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\272\320\276\320\275\321\204\320\270\320\263", 0, QApplication::UnicodeUTF8));
        btnFromFile->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \321\201 \320\264\320\270\321\201\320\272\320\260", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
