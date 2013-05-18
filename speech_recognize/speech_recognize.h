#ifndef SPEECH_RECOGNIZE_H
#define SPEECH_RECOGNIZE_H

#include <QtGui/QMainWindow>
#include "ui_speech_recognize.h"

#include "SREngine.h"
#include <string>
#include <QLabel>
#include <QString>
#include <QDebug>
#include <QDir>

#define CMD_Begin  1
#define CMD_End    2
#define CMD_Single  10
#define CMDClose 11
#define CMDShutdown 12
#define CMDShowdesktop 13
#define CMD_Group   20
#define CMDOpenComputer  21
#define CMDOpenDesktop  22
#define CMD_Options  30
#define CMDOne  31
#define CMDTwo  32
#define CMDThree  33
#define CMDPgup  34
#define CMDPgdn  35
#define CMDBack  36
#define CMD_Config  40
#define CMDYes      41
#define CMDNo       42

class speech_recognize : public QMainWindow
{
	Q_OBJECT

public:
	speech_recognize(QWidget *parent = 0, Qt::WFlags flags = 0);
	~speech_recognize();
	

signals:
	void REG(); 

public slots:
	void recordEvent();

protected:
	bool winEvent(MSG *message, long *result);
	void paintEvent(QPaintEvent *);

	void executeCommand(ISpPhrase *pPhrase, std::string dstrText);
	void initUI();
private:
	Ui::speech_recognizeClass ui;
	KSREngine m_SREngine;
	//×´Ì¬±ê¼Ç
	QLabel m_toolLabel;
	QString m_cmdLine;
	int m_cmdType;
	QDir m_cmdDir;
	int m_index;


	void cmdBegin();
	void cmdEnd();
	void command(SPPHRASE *pElements, std::string dstrText);
	void cmdConfig(SPPHRASE *pElements);
	void cmdOptios(SPPHRASE *pElements, std::string dstrText);

	void addListText(const QStringList &list, const  int index);
	void chooseNumber(QLabel* label, int number);
	QString dealString(const QString &str);
};

#endif // SPEECH_RECOGNIZE_H
