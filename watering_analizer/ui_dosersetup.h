/********************************************************************************
** Form generated from reading UI file 'dosersetup.ui'
**
** Created: Thu Jan 2 20:02:37 2014
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOSERSETUP_H
#define UI_DOSERSETUP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_dosersetup
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QRadioButton *rdOneDoser;
    QRadioButton *rdMultiDosers;
    QHBoxLayout *horizontalLayout_5;
    QRadioButton *rdDoser1;
    QRadioButton *rdDoser2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lblCupCount;
    QSpinBox *spnCount;
    QPushButton *btnStartTune;
    QSlider *slAngle;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QSpacerItem *horizontalSpacer_6;
    QLabel *lbCurrentAngle;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btSavePos;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *btnMove;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *btnTest;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnDeleteLast;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *edResult;
    QDialogButtonBox *buttonBox;
    QButtonGroup *buttonGroup;
    QButtonGroup *buttonGroup_2;

    void setupUi(QDialog *dosersetup)
    {
        if (dosersetup->objectName().isEmpty())
            dosersetup->setObjectName(QString::fromUtf8("dosersetup"));
        dosersetup->resize(681, 376);
        verticalLayout_2 = new QVBoxLayout(dosersetup);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        rdOneDoser = new QRadioButton(dosersetup);
        buttonGroup_2 = new QButtonGroup(dosersetup);
        buttonGroup_2->setObjectName(QString::fromUtf8("buttonGroup_2"));
        buttonGroup_2->addButton(rdOneDoser);
        rdOneDoser->setObjectName(QString::fromUtf8("rdOneDoser"));
        rdOneDoser->setChecked(true);

        horizontalLayout->addWidget(rdOneDoser);

        rdMultiDosers = new QRadioButton(dosersetup);
        buttonGroup_2->addButton(rdMultiDosers);
        rdMultiDosers->setObjectName(QString::fromUtf8("rdMultiDosers"));

        horizontalLayout->addWidget(rdMultiDosers);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        rdDoser1 = new QRadioButton(dosersetup);
        buttonGroup = new QButtonGroup(dosersetup);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(rdDoser1);
        rdDoser1->setObjectName(QString::fromUtf8("rdDoser1"));
        rdDoser1->setEnabled(false);
        rdDoser1->setAutoExclusive(true);

        horizontalLayout_5->addWidget(rdDoser1);

        rdDoser2 = new QRadioButton(dosersetup);
        buttonGroup->addButton(rdDoser2);
        rdDoser2->setObjectName(QString::fromUtf8("rdDoser2"));
        rdDoser2->setEnabled(false);
        rdDoser2->setAutoExclusive(true);

        horizontalLayout_5->addWidget(rdDoser2);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lblCupCount = new QLabel(dosersetup);
        lblCupCount->setObjectName(QString::fromUtf8("lblCupCount"));

        horizontalLayout_2->addWidget(lblCupCount);

        spnCount = new QSpinBox(dosersetup);
        spnCount->setObjectName(QString::fromUtf8("spnCount"));
        spnCount->setMaximumSize(QSize(80, 16777215));
        spnCount->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spnCount->setMinimum(1);

        horizontalLayout_2->addWidget(spnCount);

        btnStartTune = new QPushButton(dosersetup);
        btnStartTune->setObjectName(QString::fromUtf8("btnStartTune"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnStartTune->sizePolicy().hasHeightForWidth());
        btnStartTune->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(btnStartTune);


        verticalLayout_2->addLayout(horizontalLayout_2);

        slAngle = new QSlider(dosersetup);
        slAngle->setObjectName(QString::fromUtf8("slAngle"));
        slAngle->setMaximum(200);
        slAngle->setPageStep(5);
        slAngle->setOrientation(Qt::Horizontal);
        slAngle->setTickPosition(QSlider::TicksBelow);
        slAngle->setTickInterval(10);

        verticalLayout_2->addWidget(slAngle);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(dosersetup);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        lbCurrentAngle = new QLabel(dosersetup);
        lbCurrentAngle->setObjectName(QString::fromUtf8("lbCurrentAngle"));

        horizontalLayout_4->addWidget(lbCurrentAngle);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        label_3 = new QLabel(dosersetup);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        btSavePos = new QPushButton(dosersetup);
        btSavePos->setObjectName(QString::fromUtf8("btSavePos"));
        btSavePos->setMaximumSize(QSize(200, 16777215));

        horizontalLayout_3->addWidget(btSavePos);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        btnMove = new QPushButton(dosersetup);
        btnMove->setObjectName(QString::fromUtf8("btnMove"));

        horizontalLayout_3->addWidget(btnMove);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        btnTest = new QPushButton(dosersetup);
        btnTest->setObjectName(QString::fromUtf8("btnTest"));

        horizontalLayout_3->addWidget(btnTest);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        btnDeleteLast = new QPushButton(dosersetup);
        btnDeleteLast->setObjectName(QString::fromUtf8("btnDeleteLast"));

        horizontalLayout_3->addWidget(btnDeleteLast);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_3);

        edResult = new QLineEdit(dosersetup);
        edResult->setObjectName(QString::fromUtf8("edResult"));

        verticalLayout_2->addWidget(edResult);

        buttonBox = new QDialogButtonBox(dosersetup);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(dosersetup);
        QObject::connect(buttonBox, SIGNAL(accepted()), dosersetup, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), dosersetup, SLOT(reject()));

        QMetaObject::connectSlotsByName(dosersetup);
    } // setupUi

    void retranslateUi(QDialog *dosersetup)
    {
        dosersetup->setWindowTitle(QApplication::translate("dosersetup", "Dialog", 0, QApplication::UnicodeUTF8));
        rdOneDoser->setText(QApplication::translate("dosersetup", "\320\236\320\264\320\270\320\275 \320\264\320\276\320\267\320\260\321\202\320\276\321\200", 0, QApplication::UnicodeUTF8));
        rdMultiDosers->setText(QApplication::translate("dosersetup", "\320\224\320\262\320\260 \320\264\320\276\320\267\320\260\321\202\320\276\321\200\320\260", 0, QApplication::UnicodeUTF8));
        rdDoser1->setText(QApplication::translate("dosersetup", "\320\237\320\265\321\200\320\262\321\213\320\271 \320\264\320\276\320\267\320\260\321\202\320\276\321\200", 0, QApplication::UnicodeUTF8));
        rdDoser2->setText(QApplication::translate("dosersetup", "\320\222\321\202\320\276\321\200\320\276\320\271 \320\264\320\276\320\267\320\260\321\202\320\276\321\200", 0, QApplication::UnicodeUTF8));
        lblCupCount->setText(QApplication::translate("dosersetup", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \321\201\321\202\320\260\320\272\320\260\320\275\320\276\320\262", 0, QApplication::UnicodeUTF8));
        btnStartTune->setText(QApplication::translate("dosersetup", "\320\235\320\260\321\207\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dosersetup", "0", 0, QApplication::UnicodeUTF8));
        lbCurrentAngle->setText(QApplication::translate("dosersetup", "-", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("dosersetup", "180", 0, QApplication::UnicodeUTF8));
        btSavePos->setText(QApplication::translate("dosersetup", "\320\227\320\260\320\277\320\276\320\274\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        btnMove->setText(QApplication::translate("dosersetup", "\320\237\320\276\320\264\320\262\320\270\320\275\321\203\321\202\321\214", 0, QApplication::UnicodeUTF8));
        btnTest->setText(QApplication::translate("dosersetup", "\320\237\321\200\320\276\320\262\320\265\321\200\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        btnDeleteLast->setText(QApplication::translate("dosersetup", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\277\320\276\321\201\320\273\320\265\320\264\320\275\320\270\320\271", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dosersetup: public Ui_dosersetup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOSERSETUP_H
