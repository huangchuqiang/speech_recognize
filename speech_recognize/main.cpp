#include "speech_recognize.h"
#include <QtGui/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	speech_recognize w;
	w.show();
	return a.exec();
}
