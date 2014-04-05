/********************************************************************************
** Form generated from reading UI file 'globalstatedialog.ui'
**
** Created: Sat Jan 4 17:23:22 2014
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GLOBALSTATEDIALOG_H
#define UI_GLOBALSTATEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GlobalStateDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_3;
    QLineEdit *edCalibrate;
    QPushButton *btnCalibrate;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_8;
    QLineEdit *edNightWDFrom;
    QLabel *label_6;
    QLineEdit *edNightWDTo;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_9;
    QLineEdit *edNightWeFrom;
    QLabel *label_7;
    QLineEdit *edNightWETo;
    QPushButton *btnSetNight;
    QHBoxLayout *horizontalLayout;
    QRadioButton *rdStart;
    QLabel *lblNight;
    QRadioButton *rdStop;
    QVBoxLayout *verticalLayout;
    QListWidget *lsvErrors;
    QPushButton *btnResetErrors;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *edTime;
    QPushButton *btnSetTime;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *edTime_2;
    QPushButton *btSetInterval;
    QLabel *label;
    QPushButton *btRefresh;

    void setupUi(QDialog *GlobalStateDialog)
    {
        if (GlobalStateDialog->objectName().isEmpty())
            GlobalStateDialog->setObjectName(QString::fromUtf8("GlobalStateDialog"));
        GlobalStateDialog->resize(610, 433);
        buttonBox = new QDialogButtonBox(GlobalStateDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 400, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        formLayoutWidget = new QWidget(GlobalStateDialog);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(5, 10, 371, 381));
        gridLayout = new QGridLayout(formLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        edCalibrate = new QLineEdit(formLayoutWidget);
        edCalibrate->setObjectName(QString::fromUtf8("edCalibrate"));

        verticalLayout_3->addWidget(edCalibrate);

        btnCalibrate = new QPushButton(formLayoutWidget);
        btnCalibrate->setObjectName(QString::fromUtf8("btnCalibrate"));

        verticalLayout_3->addWidget(btnCalibrate);


        gridLayout->addLayout(verticalLayout_3, 5, 1, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_8 = new QLabel(formLayoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_5->addWidget(label_8);

        edNightWDFrom = new QLineEdit(formLayoutWidget);
        edNightWDFrom->setObjectName(QString::fromUtf8("edNightWDFrom"));

        horizontalLayout_5->addWidget(edNightWDFrom);

        label_6 = new QLabel(formLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_5->addWidget(label_6);

        edNightWDTo = new QLineEdit(formLayoutWidget);
        edNightWDTo->setObjectName(QString::fromUtf8("edNightWDTo"));

        horizontalLayout_5->addWidget(edNightWDTo);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_9 = new QLabel(formLayoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_6->addWidget(label_9);

        edNightWeFrom = new QLineEdit(formLayoutWidget);
        edNightWeFrom->setObjectName(QString::fromUtf8("edNightWeFrom"));

        horizontalLayout_6->addWidget(edNightWeFrom);

        label_7 = new QLabel(formLayoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_6->addWidget(label_7);

        edNightWETo = new QLineEdit(formLayoutWidget);
        edNightWETo->setObjectName(QString::fromUtf8("edNightWETo"));

        horizontalLayout_6->addWidget(edNightWETo);


        verticalLayout_2->addLayout(horizontalLayout_6);

        btnSetNight = new QPushButton(formLayoutWidget);
        btnSetNight->setObjectName(QString::fromUtf8("btnSetNight"));

        verticalLayout_2->addWidget(btnSetNight);


        gridLayout->addLayout(verticalLayout_2, 3, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        rdStart = new QRadioButton(formLayoutWidget);
        rdStart->setObjectName(QString::fromUtf8("rdStart"));

        horizontalLayout->addWidget(rdStart);

        lblNight = new QLabel(formLayoutWidget);
        lblNight->setObjectName(QString::fromUtf8("lblNight"));
        lblNight->setEnabled(true);

        horizontalLayout->addWidget(lblNight);

        rdStop = new QRadioButton(formLayoutWidget);
        rdStop->setObjectName(QString::fromUtf8("rdStop"));

        horizontalLayout->addWidget(rdStop);


        gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lsvErrors = new QListWidget(formLayoutWidget);
        lsvErrors->setObjectName(QString::fromUtf8("lsvErrors"));

        verticalLayout->addWidget(lsvErrors);

        btnResetErrors = new QPushButton(formLayoutWidget);
        btnResetErrors->setObjectName(QString::fromUtf8("btnResetErrors"));

        verticalLayout->addWidget(btnResetErrors);


        gridLayout->addLayout(verticalLayout, 4, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        edTime = new QLineEdit(formLayoutWidget);
        edTime->setObjectName(QString::fromUtf8("edTime"));

        horizontalLayout_3->addWidget(edTime);

        btnSetTime = new QPushButton(formLayoutWidget);
        btnSetTime->setObjectName(QString::fromUtf8("btnSetTime"));

        horizontalLayout_3->addWidget(btnSetTime);


        gridLayout->addLayout(horizontalLayout_3, 1, 1, 1, 1);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        label_5 = new QLabel(formLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        edTime_2 = new QLineEdit(formLayoutWidget);
        edTime_2->setObjectName(QString::fromUtf8("edTime_2"));

        horizontalLayout_4->addWidget(edTime_2);

        btSetInterval = new QPushButton(formLayoutWidget);
        btSetInterval->setObjectName(QString::fromUtf8("btSetInterval"));

        horizontalLayout_4->addWidget(btSetInterval);


        gridLayout->addLayout(horizontalLayout_4, 2, 1, 1, 1);

        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        btRefresh = new QPushButton(formLayoutWidget);
        btRefresh->setObjectName(QString::fromUtf8("btRefresh"));

        gridLayout->addWidget(btRefresh, 5, 0, 1, 1);


        retranslateUi(GlobalStateDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), GlobalStateDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), GlobalStateDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(GlobalStateDialog);
    } // setupUi

    void retranslateUi(QDialog *GlobalStateDialog)
    {
        GlobalStateDialog->setWindowTitle(QApplication::translate("GlobalStateDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        btnCalibrate->setText(QApplication::translate("GlobalStateDialog", "\320\232\320\260\320\273\320\270\320\261\321\200\320\276\320\262\320\272\320\260", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("GlobalStateDialog", "\320\277\320\275-\320\277\321\202", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("GlobalStateDialog", "-", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("GlobalStateDialog", "\321\201\320\261-\320\262\321\201", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("GlobalStateDialog", "-", 0, QApplication::UnicodeUTF8));
        btnSetNight->setText(QApplication::translate("GlobalStateDialog", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        rdStart->setText(QApplication::translate("GlobalStateDialog", "\320\241\321\202\320\260\321\200\321\202", 0, QApplication::UnicodeUTF8));
        lblNight->setText(QString());
        rdStop->setText(QApplication::translate("GlobalStateDialog", "\320\241\321\202\320\276\320\277", 0, QApplication::UnicodeUTF8));
        btnResetErrors->setText(QApplication::translate("GlobalStateDialog", "\320\241\320\261\321\200\320\276\321\201 \320\276\321\210\320\270\320\261\320\276\320\272", 0, QApplication::UnicodeUTF8));
        btnSetTime->setText(QApplication::translate("GlobalStateDialog", "\320\230\321\201\320\277\321\200\320\260\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("GlobalStateDialog", "\320\222\321\200\320\265\320\274\321\217", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("GlobalStateDialog", "\320\236\321\210\320\270\320\261\320\272\320\270", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("GlobalStateDialog", "\320\230\320\275\321\202\320\265\321\200\320\262\320\260\320\273 \320\277\321\200\320\276\320\262\320\265\321\200\320\272\320\270", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("GlobalStateDialog", "\320\235\320\276\321\207\321\214", 0, QApplication::UnicodeUTF8));
        btSetInterval->setText(QApplication::translate("GlobalStateDialog", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GlobalStateDialog", "\320\241\320\276\321\201\321\202\320\276\321\217\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        btRefresh->setText(QApplication::translate("GlobalStateDialog", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GlobalStateDialog: public Ui_GlobalStateDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GLOBALSTATEDIALOG_H
