#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <QApplication>
#include <QFontDialog>
#include <QString>
#include "../dispobjmanager/mainwidget.h"
#include "../tester.h"
#include "../dispobject/dispobject.h"
#include "../dispobjfilter/dispobjfilter.h"
#include "../cuser/cuser.h"
#include "../udatabase/udatabase.h"
#include "../dispobjmanager/mainwidget.h"
#include "../dispobjmanager/picturewindow/picwincontrol.h"
#include "../dispobjmanager/picturewindow/picturewindow.h"
#include "../dispobjmanager/textwindow/txtwincontrol.h"
#include "../dispobjmanager/textwindow/textwindow.h"



MainWidget * mw;
DispObjFilter * dof;
UDataBase *udb;

using namespace std;

static void createDispObjs (void)
{
	DispObject * dobj = NULL;
	QString fname = "";
	int i = 0;
	CUser * user = NULL;
	QString nick = QString ("Agu");
	QString MAC = QString("AB:DC:EF:56:32:15");
	

	/* Cargamos las fotos y los textos */
	/*! primero los textos */
	for (i = 1; i < 11; i++ ) {
		fname = "";
		fname.setNum(i);
		fname.prepend("mesaje");
		fname.append (".txt");
		cout << "Agregando archivo: " << qstrtochar (fname) << endl;
		user = new CUser(&nick, &MAC);
		dobj = new DispObject (fname);
		dobj->setUser(user);
		/* preguntamos si lo aceptamos */
		if (dof->accept(dobj)) {
			cout << "Aceptamos el archivo: " << i << endl;
			/*! entonces lo mandamos a la MainWidget */
			mw->addDispObject(dobj);
		}
		else {
			cout << "Rechazamos el archivo: " << i << endl;
		}
	}

	for (i = 1; i < 11; i++ ) {
		fname = "";
		fname.setNum(i);
		fname.prepend("foto");
		fname.append (".jpg");
		cout << "Agregando archivo: " << qstrtochar (fname) << endl;
		user = new CUser(&nick, &MAC);
		dobj = new DispObject (fname);
		dobj->setUser(user);
		/* preguntamos si lo aceptamos */
		if (dof->accept(dobj)) {
			cout << "Aceptamos el archivo: " << i << endl;
			/*! entonces lo mandamos a la MainWidget */
			mw->addDispObject(dobj);
		}
		else {
			cout << "Rechazamos el archivo: " << i << endl;
		}
		
	}
	printf ("terminamos de agregar\n");
	
	
}


int main (int argc, char ** argv)
{	
	QApplication app(argc, argv, true);
	QString fname = " ";
	mw = new MainWidget (NULL ,fname);
	bool result = false;
	
	
	udb = new UDataBase("pruebaextra.txt");	
	result = udb->loadFromFile ("pruebaextra.txt");
	
	dof = new DispObjFilter (udb);
	fail_if (dof == NULL);
	
	createDispObjs();
	
	app.exec();
	delete mw;
	return 0;
}