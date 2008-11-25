#include <stdio.h>
#include <stdlib.h>
#include <QString>


#include "userlist.h"
#include "userobject.h"



int main () {
	UserList *usrlist = new UserList();
	UserObject *usr = NULL;
	QString dummy("userlist.defaul");
	QString *data = NULL;
	
	for (int i = 0; i < 10; i++) {
		usr = new UserObject();
		data = new QString ("DAAAAAMMYYYYY");
		usr->setName (*data);
		delete data;
		data = new QString ("DAAAAAMMYYYYY");
		usr->setNumber (*data);
		delete data;
		usrlist->insertUser (usr);
	}
	
	printf ("************BEFORE*************\n");
	printf ("Tamaño: %d\n", usrlist->getSize());
	usrlist->toFile (dummy);
	printf ("************AFTER*************\n");
	/*usrlist->printList();*/
	delete usrlist;
	usrlist = new UserList();
	usrlist->fromFile (dummy);
	usrlist->printList();
	printf ("Tamaño: %d\n", usrlist->getSize());
	printf ("*************************\n");
	
	delete usrlist;
	return 0;
}

