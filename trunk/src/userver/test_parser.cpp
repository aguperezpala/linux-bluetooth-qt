#include <stdio.h>
#include <QString>
#include "../cuser/cuser.h"
#include "../udatabase/udatabase.h"
#include "usparser.h"
#include "userver.h"
#include "../tester.h"
#include "../consts.h"

UDataBase * db;
USParser parser;
QString req;

static void check_isValidRequest (void)
{
	
	
	req = "a";
	fail_if (parser.isValidRequest (req) != -1);
	req = "<SSAP";
	fail_if (parser.isValidRequest (req) != -1);
	req = "<SSAP>";
	fail_if (parser.isValidRequest (req) != -1);
	req = "a<SSAP>";
	fail_if (parser.isValidRequest (req) != -2);
	req = "<SSAP>a";
	fail_if (parser.isValidRequest (req) != -1);
	req = "<SSAP>register<SSAP";
	fail_if (parser.isValidRequest (req) != -1);
	req = "<SSAP>registrar<SSAP>";
	fail_if (parser.isValidRequest (req) != 1);
	req = "<SSAP>av:de:as:tr:01:sapepe";
	fail_if (parser.isValidRequest (req) != -1);
	req = "<SSAP>av:de:as:tr:01:sapepe<SSAP";
	fail_if (parser.isValidRequest (req) != -1);
	req = "<SSAP>av:de:as:tr:01:DFsapepe<SSAP>";
	fail_if (parser.isValidRequest (req) != -2);
	req = "<SSAP>av:de:as:tr:01:DF,sapepe<SSAP>";
	fail_if (parser.isValidRequest (req) != 0);
	
	req = "<SSAP>av:de:as:tr:01:DF,sapepe<SSAP>"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	fail_if (parser.isValidRequest (req) != -2);
	
	
}

static void check_parseRequest (void)
{
	CUser * user = NULL;
	
	req = "<SSAP>av:de:as:tr:01:DF,sapepe<SSAP>";
	user = parser.parseRequest (req);
	fail_if (user->getMAC() != "av:de:as:tr:01:DF");
	fail_if (user->getNick() != "sapepe");
	printf ("USER:\n");
	user->printUser();
	delete user;
	req = "<SSAP>av:de:as:tr:01:DF,<SSAP>";
	user = parser.parseRequest (req);
	fail_if (user->getMAC() != "av:de:as:tr:01:DF");
	fail_if (user->getNick() != "");
	printf ("USER:\n");
	user->printUser();
	delete user;
}


static void check_createResponse ()
{
	req = "ok";
	
	printf ("before req: %s\n", qstrtochar(req));
	fail_if (parser.createResponse (req) == false);
	printf ("after req: %s\n", qstrtochar(req));
	fail_if (req != "<SSAP>ok<SSAP>");
	req = "<SSAP>av:de:as:tr:01:DF,sapepe<SSAP>"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	fail_if (parser.createResponse (req) == true);
	fail_if (req != "<SSAP>av:de:as:tr:01:DF,sapepe<SSAP>"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}
	


int main (void)
{
	CUser * user = NULL;
	QString nick = QString ("MAMAAAAAA");
	QString MAC = QString("XX:XX:XX:56:32:15");
	
	user = new CUser(&nick, &MAC);
	db = new UDataBase("pruebaextra.txt");	
	fail_if (db == NULL);
	fail_if (db->existUser(user));
	fail_if(db->loadFromFile ("pruebaextra.txt") == false);
	
	/* chequeamos que cumpla con el protocolo */
	check_isValidRequest();
	
	/* chequeamos el parseo de usuarios */
	check_parseRequest ();
	
	/* chequeamos la creacion de los responses */
	check_createResponse();
	
	delete db;
	
	return 0;
}