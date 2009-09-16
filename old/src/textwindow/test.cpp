
#include <stdio.h>
#include <QApplication>
#include <QString>
#include "textwindow.h"
#include "../../tester.h"

TextWindow * txtw;

QString * getmsg (void)
{
	static int counter = 6;
	
	counter--;
	if (counter >= 0) {
		QString * pepe = new QString("hasta el remil choripan bolo");
		printf ("llegamos aca\n");
		return pepe;
	} else
		return NULL;
}
int main (int argc, char ** argv)
{	
	QApplication app(argc, argv, true);
	QString test = "mamamamamamamamamamam :) jajajaja :p";
	
	
	txtw = new TextWindow (&getmsg);
	txtw->pause(false);
	txtw->setMesg (test);
	txtw->show();
	
	return app.exec();
}