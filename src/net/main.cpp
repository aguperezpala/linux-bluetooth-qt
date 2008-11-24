#include <stdio.h>
#include <stdlib.h>
#include <QString>

#include "userserver.h"
#include "../userspp/userlist.h"



int main () {
	UserList *usrlist = new UserList();
	UserServer server(usrlist);/*!burrar el count en server.start()*/
	
	server.startListen();
	server.start();

	usrlist->printList();
	delete usrlist;
	return 0;
}

