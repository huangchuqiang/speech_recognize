/********************************************************************************
** Form generated from reading UI file 'speech_recognize.ui'
**
** Created: Thu May 16 10:19:35 2013
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
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_speech_recognizeClass
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *lab_cmd;
    QLabel *lab_config;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *speech_recognizeClass)
    {
        if (speech_recognizeClass->objectName().isEmpty())
            speech_recognizeClass->setObjectName(QString::fromUtf8("speech_recognizeClass"));
        speech_recognizeClass->resize(444, 254);
        centralWidget = new QWidget(speech_recognizeClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 441, 61));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lab_cmd = new QLabel(verticalLayoutWidget);
        lab_cmd->setObjectName(QString::fromUtf8("lab_cmd"));

        verticalLayout->addWidget(lab_cmd);

        lab_config = new QLabel(verticalLayoutWidget);
        lab_config->setObjectName(QString::fromUtf8("lab_config"));

        verticalLayout->addWidget(lab_config);

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
        lab_cmd->setText(QApplication::translate("speech_recognizeClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        lab_config->setText(QApplication::translate("speech_recognizeClass", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class speech_recognizeClass: public Ui_speech_recognizeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPEECH_RECOGNIZE_H
