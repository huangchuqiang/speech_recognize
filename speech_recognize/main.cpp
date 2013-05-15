#include "speech_recognize.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
	speech_recognize w;
	w.show();
	return a.exec();
}
