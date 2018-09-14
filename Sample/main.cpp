#include "testwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TestWidget w;
	if(QApplication::arguments().contains(QStringLiteral("--autostart")))
		w.setAutoStarted();
	w.show();

	return a.exec();
}
