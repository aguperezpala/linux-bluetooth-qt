
#include <stdio.h>
#include <unistd.h>
#include <QApplication>
#include <QFontDialog>
#include <QString>
#include <QPixmap>
#include "picturewindow.h"
#include "../../tester.h"


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
	PictureWindow * pw = new PictureWindow(picture_sender);
	
	pw->setPicture();
	pw->show();
	app.exec();
	delete pw;
	return 0;
}