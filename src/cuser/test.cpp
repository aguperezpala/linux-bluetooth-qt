#include <stdio.h>
#include "cuser.h"

int main (void)
{
	CUser * user = NULL;
	QString nick = QString ("Agu");
	QString MAC = QString("AB:DC:EF:56:32:15");
	
	user = new CUser(&nick, &MAC);
	user->printUser();
	
	delete user;
	
	return 0;
}