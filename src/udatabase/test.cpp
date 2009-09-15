#include <stdio.h>
#include "udatabase.h"
#include "../cuser/cuser.h"
#include "../tester.h"

int main (void)
{
	CUser * user = NULL;
	UDataBase * db = NULL;
	CUser * user2 = NULL;
	QString nick = QString ("MAMAAAAAA");
	QString MAC = QString("XX:XX:XX:56:32:15");
	QString nick2, MAC2;
	
	user = new CUser(&nick, &MAC);
	user2 = new CUser(&nick, &MAC);
	
	
	db = new UDataBase("prueba.txt");
	fail_if (db == NULL);
	fail_if (db->existUser(user));
	db->addUser(user);
	fail_unless(db->existUser(user));
	fail_unless(db->existUser(user2));
	db->addUser(user2);
	
	db->print();
	db->saveToFile();
	
	delete db;
	
	
	db = new UDataBase("pruebaextra.txt");
	fail_if(db->loadFromFile ("pruebaextra.txt") == false);
	db->clean();
	printf ("\n*********************\n");
	db->print();
	printf ("\n*********************\n");
	user = new CUser(&nick, &MAC);
	/*fail_unless(db->existUser(user));*/
	printf ("cargada de archivo\n");
	db->print();
	db->addUser(user);
	/*db->saveToFile();*/
	
	delete db;
	
	return 0;
}