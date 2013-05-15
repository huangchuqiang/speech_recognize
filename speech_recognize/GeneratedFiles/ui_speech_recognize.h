/********************************************************************************
** Form generated from reading UI file 'speech_recognize.ui'
**
** Created: Tue May 14 22:12:52 2013
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
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_speech_recognizeClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *speech_recognizeClass)
    {
        if (speech_recognizeClass->objectName().isEmpty())
            speech_recognizeClass->setObjectName(QString::fromUtf8("speech_recognizeClass"));
        speech_recognizeClass->resize(598, 395);
        centralWidget = new QWidget(speech_recognizeClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        speech_recognizeClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(speech_recognizeClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 598, 21));
        speech_recognizeClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(speech_recognizeClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        speech_recognizeClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(speech_recognizeClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        speech_recognizeClass->setStatusBar(statusBar);

        retranslateUi(speech_recognizeClass);

        QMetaObject::connectSlotsByName(speech_recognizeClass);
    } // setupUi

    void retranslateUi(QMainWindow *speech_recognizeClass)
    {
        speech_recognizeClass->setWindowTitle(QApplication::translate("speech_recognizeClass", "speech_recognize", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class speech_recognizeClass: public Ui_speech_recognizeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPEECH_RECOGNIZE_H
