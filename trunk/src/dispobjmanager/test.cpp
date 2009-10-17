#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <QApplication>
#include <QFontDialog>
#include <QString>
#include "mainwidget.h"
#include "../tester.h"
#include "../dispobject/dispobject.h"

MainWidget * mw;
using namespace std;

static void createDispObjs (void)
{
	DispObject * dobj = NULL;
	QString fname = "";
	QString dir = "mensajes/";
	int i = 0;
	
	for (i = 1; i < 27 ; i++ ) {
		fname = "";
		fname.setNum(i);
		fname.prepend(dir);
		cout << "Agregando archivo: " << qstrtochar (fname) << endl;
		dobj = new DispObject (fname);
		mw->addDispObject (dobj);
	}
	
	for (i = 1; i < 11; i++ ) {
		fname = "";
		fname.setNum(i);
		fname.prepend("foto");
		fname.prepend(dir);
		cout << "Agregando archivo: " << qstrtochar (fname) << endl;
		dobj = new DispObject (fname);
		mw->addDispObject (dobj);
	}
	printf ("terminamos de agregar\n");
	
	
}


int main (int argc, char ** argv)
{	
	QApplication app(argc, argv, true);
	QString fname = "configfile.conf";
	mw = new MainWidget (NULL ,fname);
	
	/* creamos ahora los disp objects y los metemos */
	createDispObjs();
	
	
	app.exec();
	delete mw;
	return 0;
}