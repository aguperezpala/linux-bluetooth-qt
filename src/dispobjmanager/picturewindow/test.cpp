
#include <stdio.h>
#include <unistd.h>
#include <QApplication>
#include <QFontDialog>
#include <QString>
#include "textwindow.h"
#include "../../tester.h"

TextWindow * txtw;

QString * getmsg (void)
{
	static int counter = 3;
	
	counter--;
	if (counter >= 0) {
		QString * pepe = new QString("hasta el remil choripan bolo");
		printf ("llegamos aca\n");
		return pepe;
	} else
		return NULL;
}
static void test_setFont (void)
{
	bool ok = false;
	
	QFont font = QFontDialog::getFont(&ok,txtw->font(),txtw);
	if (ok) 
		txtw->setTextFont (font);
	
}
int main (int argc, char ** argv)
{	
	QApplication app(argc, argv, true);
	QString test = "mamamamamamamamamamam :) jajajaja :p";
	
	
	txtw = new TextWindow (&getmsg);
	txtw->pause(false);
	txtw->setMesg (test);
	txtw->show();
	printf ("esperamos\n");
	
	
	test_setFont();
	printf ("esperamos de nuevo\n");
	txtw->setMesg(test);
	app.exec();
	delete txtw;
	return 0;
}