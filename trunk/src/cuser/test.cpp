#include <stdio.h>
#include "cuser.h"
#include "../tester.h"

int main (void)
{
	CUser * user = NULL;
	CUser * user2 = NULL;
	QString nick = QString ("Agu");
	QString MAC = QString("AB:DC:EF:56:32:15");
	QString nick2, MAC2;
	
	user = new CUser(&nick, &MAC);
	fail_if (user == NULL);
	nick2 = user->getNick();
	MAC2 = user->getMAC();
	
	/* chequeamos que tengan el mismo nick */
	fail_if (nick != nick2);
	
	/* chequeamos que tengan la misma MAC */
	fail_if (MAC != MAC2);
	
	/* creamos otro usuario y chequeamos que sean iguales */
	user2 = new CUser(&nick2, &MAC2);
	
	fail_if(*user2 != *user);
	fail_unless(*user == *user2);
	
	user->printUser();
	user2->printUser();
	
	delete user;
	delete user2;
	
	return 0;
}