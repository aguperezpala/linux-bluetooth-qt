
#include <stdio.h>
#include <unistd.h>
#include <QApplication>
#include <QFontDialog>
#include <QString>
#include "picturewindow.h"
#include "picwincontrol.h"
#include "../../tester.h"

PictureWindow * pw;
PicWinControl * pwc;

QPixmap * picture_sender (void)
{
	QPixmap * pic = new QPixmap();
	QString fname = "m4.jpg";
	
	pic->load (fname);
	printf ("Cambiando de foto....\n");
	return pic;
}


int main (int argc, char ** argv)
{	
	QApplication app(argc, argv, true);
	QString test = "mamamamamamamamamamam :) jajajaja :p";
	Qt::WindowFlags flags = 0;
	
	flags = Qt::FramelessWindowHint;
	
	printf ("LLEGAMOS1\n");
	
	
	pw = new PictureWindow (&picture_sender);
	printf ("LLEGAMOS2\n");
	pwc = new PicWinControl(0, test, pw);
	printf ("LLEGAMOS3\n");
	pw->setWindowFlags (flags);
	printf ("LLEGAMOS4\n");
	pwc->show();
	pw->pause(false);
	pw->setPicture ();
	pw->show();
	printf ("esperamos\n");
	
	
	/*test_setFont();*/
	printf ("esperamos de nuevo\n");
	
	app.exec();
	delete pw;
	delete pwc;
	return 0;
}