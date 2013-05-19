/********************************************************************************
** Form generated from reading UI file 'speech_recognize.ui'
**
** Created: Sat May 18 22:59:58 2013
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPEECH_RECOGNIZE_H
#define UI_SPEECH_RECOGNIZE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_speech_recognizeClass
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *lab_title;
    QLabel *lab_cmd;
    QLabel *lab_config;
    QVBoxLayout *verticalLayout_2;
    QLabel *lab_caption;
    QLabel *lab_next;
    QLabel *lab_1;
    QLabel *lab_2;
    QLabel *lab_3;
    QSpacerItem *verticalSpacer;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *speech_recognizeClass)
    {
        if (speech_recognizeClass->objectName().isEmpty())
            speech_recognizeClass->setObjectName(QString::fromUtf8("speech_recognizeClass"));
        speech_recognizeClass->resize(446, 252);
        centralWidget = new QWidget(speech_recognizeClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(8, 0, 431, 231));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lab_title = new QLabel(layoutWidget);
        lab_title->setObjectName(QString::fromUtf8("lab_title"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        lab_title->setFont(font);
        lab_title->setLayoutDirection(Qt::LeftToRight);
        lab_title->setTextFormat(Qt::PlainText);
        lab_title->setAlignment(Qt::AlignCenter);
        lab_title->setMargin(-1);

        verticalLayout->addWidget(lab_title);

        lab_cmd = new QLabel(layoutWidget);
        lab_cmd->setObjectName(QString::fromUtf8("lab_cmd"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font1.setPointSize(14);
        lab_cmd->setFont(font1);

        verticalLayout->addWidget(lab_cmd);

        lab_config = new QLabel(layoutWidget);
        lab_config->setObjectName(QString::fromUtf8("lab_config"));
        lab_config->setFont(font1);

        verticalLayout->addWidget(lab_config);


        verticalLayout_3->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lab_caption = new QLabel(layoutWidget);
        lab_caption->setObjectName(QString::fromUtf8("lab_caption"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font2.setPointSize(10);
        lab_caption->setFont(font2);

        verticalLayout_2->addWidget(lab_caption);

        lab_next = new QLabel(layoutWidget);
        lab_next->setObjectName(QString::fromUtf8("lab_next"));
        lab_next->setFont(font2);

        verticalLayout_2->addWidget(lab_next);

        lab_1 = new QLabel(layoutWidget);
        lab_1->setObjectName(QString::fromUtf8("lab_1"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font3.setPointSize(12);
        lab_1->setFont(font3);

        verticalLayout_2->addWidget(lab_1);

        lab_2 = new QLabel(layoutWidget);
        lab_2->setObjectName(QString::fromUtf8("lab_2"));
        lab_2->setFont(font3);

        verticalLayout_2->addWidget(lab_2);

        lab_3 = new QLabel(layoutWidget);
        lab_3->setObjectName(QString::fromUtf8("lab_3"));
        lab_3->setFont(font3);

        verticalLayout_2->addWidget(lab_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        verticalLayout_3->addLayout(verticalLayout_2);

        speech_recognizeClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(speech_recognizeClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        speech_recognizeClass->setStatusBar(statusBar);

        retranslateUi(speech_recognizeClass);

        QMetaObject::connectSlotsByName(speech_recognizeClass);
    } // setupUi

    void retranslateUi(QMainWindow *speech_recognizeClass)
    {
        speech_recognizeClass->setWindowTitle(QApplication::translate("speech_recognizeClass", "speech_recognize", 0, QApplication::UnicodeUTF8));
        lab_title->setText(QApplication::translate("speech_recognizeClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        lab_cmd->setText(QApplication::translate("speech_recognizeClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        lab_config->setText(QApplication::translate("speech_recognizeClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        lab_caption->setText(QApplication::translate("speech_recognizeClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        lab_next->setText(QApplication::translate("speech_recognizeClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        lab_1->setText(QApplication::translate("speech_recognizeClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        lab_2->setText(QApplication::translate("speech_recognizeClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        lab_3->setText(QApplication::translate("speech_recognizeClass", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class speech_recognizeClass: public Ui_speech_recognizeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPEECH_RECOGNIZE_H
