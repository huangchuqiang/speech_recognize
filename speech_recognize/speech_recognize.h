#ifndef SPEECH_RECOGNIZE_H
#define SPEECH_RECOGNIZE_H

#include <QtGui/QMainWindow>
#include "ui_speech_recognize.h"

#include <sapi.h>
#include <sphelper.h>
#include <spuihelp.h>
#include <QLabel>


class speech_recognize : public QMainWindow
{
	Q_OBJECT

public:
	speech_recognize(QWidget *parent = 0, Qt::WFlags flags = 0);
	~speech_recognize();
	

signals:
	void REG(); 

public slots:
	void doSomeThing();

protected:
	bool winEvent(MSG *message, long *result);
private:
	Ui::speech_recognizeClass ui;
	QLabel label;

	CComPtr<ISpRecognizer> m_pRecognizer;
	CComPtr<ISpRecoContext> m_pRecoCtxt;
	CComPtr<ISpRecoGrammar> m_pCmdGram;
	CComPtr<ISpAudio> m_pAudio;

};

#endif // SPEECH_RECOGNIZE_H
