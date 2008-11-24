#include <stdio.h>
#include <stdlib.h>
#include <QString>


#include "userlist.h"
#include "userobject.h"



int main () {
	UserList *usrlist = new UserList();
	UserObject *usr = NULL;
	QString *data = NULL;
	
	for (int i = 0; i < 99; i++) {
		usr = new UserObject();
		data = new QString ("DAAAAAMMYYYYY");
		usr->setName (*data);
		delete data;
		data = new QString ("DAAAAAMMYYYYY");
		usr->setNumber (*data);
		delete data;
		usrlist->insertUser (usr);
	}
		


	usrlist->printList();
	delete usrlist;
	return 0;
}

