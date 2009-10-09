
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <QApplication>
#include <QFontDialog>
#include <QFile>
#include <QString>

#include "dispobjfilter.h"
#include "../dispobject/dispobject.h"
#include "../cuser/cuser.h"
#include "../udatabase/udatabase.h"
#include "../tester.h"


UDataBase *udb;
DispObjFilter * dof;
DispObject * dobj;
QString fname;

using namespace std;

static void createDispObjs (void)
{
	DispObject * dobj = NULL;
	QString fname = "";
	int i = 0;
	CUser * user = NULL;
	QString nick = QString ("Agu");
	QString MAC = QString("AB:DC:EF:56:32:15");
	
	/*
	for (i = 1; i < 27 ; i++ ) {
		fname = "";
		fname.setNum(i);
		fname.prepend(dir);
		cout << "Agregando archivo: " << qstrtochar (fname) << endl;
		dobj = new DispObject (fname);
		mw->addDispObject (dobj);
	}
	*/
	for (i = 1; i < 11; i++ ) {
		fname = "";
		fname.setNum(i);
		fname.prepend("foto");
		cout << "Agregando archivo: " << qstrtochar (fname) << endl;
		user = new CUser(&nick, &MAC);
		dobj = new DispObject (fname);
		dobj->setUser(user);
		/* preguntamos si lo aceptamos */
		if (dof->accept(dobj)) {
			cout << "Aceptamos el archivo: " << i << endl;
			delete dobj;
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
	CUser * user = NULL;
	QString nick = QString ("Agu");
	QString MAC = QString("AB:DC:EF:56:32:15");
	QString data;
	bool result;
	
	
	user = new CUser(&nick, &MAC);
	dof = NULL;
	
	udb = new UDataBase("pruebaextra.txt");	
	result = udb->loadFromFile ("pruebaextra.txt");
	
	dof = new DispObjFilter (udb);
	fail_if (dof == NULL);
	
	createDispObjs();
	/*
	fname = "msjtext";
	dobj = new DispObject (fname);
	dobj->setUser(user->copy());
	dobj->kind = DISPOBJ_TEXT;
	fail_unless (dobj->file.open (QIODevice::ReadOnly));
	data = dobj->file.readAll();
	dobj->setData (data);
	dobj->file.close();
	
	fail_if (dof->accept (dobj));
	
	
	fname = "msjtext1";
	dobj = new DispObject (fname);
	dobj->setUser(user->copy());
	dobj->kind = DISPOBJ_TEXT;
	fail_unless (dobj->file.open (QIODevice::ReadOnly));
	data = dobj->file.readAll();
	dobj->setData (data);
	udb->addUser(user->copy());
	dobj->file.close();*/
	/* cancelado por el usuario */
	/*fail_if (dof->accept (dobj));
	
	
	fname = "msjtext2";
	dobj = new DispObject (fname);
	dobj->setUser(user->copy());
	dobj->kind = DISPOBJ_TEXT;
	fail_unless (dobj->file.open (QIODevice::ReadOnly));
	data = dobj->file.readAll();
	dobj->setData (data);
	dobj->file.close();*/
	/* ACCEPTADO por el usuario */
	/*fail_unless (dof->accept (dobj));
	
	
	delete udb;
	delete user;*/
	delete dof;
	return 0;
}