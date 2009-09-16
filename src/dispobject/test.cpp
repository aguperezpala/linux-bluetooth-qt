#include <stdio.h>
#include "dispobject.h"
#include "../tester.h"
#include "../consts.h"
#include "../cuser/cuser.h"



int main (void)
{
	DispObject d;
	CUser * user = NULL;
	CUser * user2 = NULL;
	QString nick = QString ("Agu");
	QString MAC = QString("AB:DC:EF:56:32:15");
	QString nick2, MAC2;
	QString data = "mama!";
	QString data2;
	
	user = new CUser(&nick, &MAC);
	nick2 = user->getNick();
	MAC2 = user->getMAC();
	user2 = new CUser(&nick2, &MAC2);
	
	d.kind = DISPOBJ_TEXT;
	fail_unless (d.kind == DISPOBJ_TEXT);
	
	d.setUser(user2);
	fail_if ((d.getUser()) == user);
	fail_unless((d.getUser()) == user2);	
	d.setUser(user);
	
	d.setData (data);
	data2 = d.getData();
	fail_unless (data == data2);
	data2 = "";
	fail_unless (d.getData() != "");
	d.setData (data);
	printf ("DATA: %s\n", qstrtochar(d.getData()));
	data = "mamamamama";
	printf ("DATA: %s\n", qstrtochar(data));
	fail_unless (d.getData() == "mama!");
	
	return 0;
}