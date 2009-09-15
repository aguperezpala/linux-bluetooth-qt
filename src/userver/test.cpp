#include <stdio.h>
#include <QString>
#include "../cuser/cuser.h"
#include "../udatabase/udatabase.h"
#include "usparser.h"
#include "userver.h"
#include "../tester.h"
#include "../consts.h"

UDataBase * db;
UServer * server;
QString req;
int sock;
QString readbuff;
QString writebuff;

static void connect_server (int a, int b)
{
	int i = 0;
	struct sockaddr_in addr;
	bool connected = false;
	
	sock = -1;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	assert (sock >= 0);
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr ("127.0.0.1");
	
	/*! ¿¿¿ Esto funciona ??? Revisar "man inet_addr" */
	
	
	for (i = a; i <= b && !connected; i++) {
		addr.sin_port = htons (i);
		if (connect (sock, (struct sockaddr *)&addr, sizeof(addr)) == 0)
		{
			printf ("Cliente: CONECTADO port:%d\n",i);
			connected = true;
		}
		printf ("Cliente: tratando al puerto: %d\n", i);
	}
	if (!connected){
		printf ("Error al conectarse el cliente\n");
		exit (1);
	}
	
}

static void check_register ()
{
	int writeBytes = 0;
	int readBytes = 0;
	char readb[300];
	
	connect_server (1234, 1238);
	writebuff = "hola papa";
	writeBytes = write (sock, qstrtochar(writebuff), writebuff.length());
	readBytes = read (sock, readb, 300);
	/* nos deberia haber rechazado la conexion */
	printf ("Cliente %d: Leimos: %d\n", __LINE__, readBytes);
	fail_unless (readBytes <= 0);
	
	/* nos conectamos de nuevo y ahora nos deberia aceptar */
	connect_server (1234, 1238);
	writebuff = "<SSAP>registrar<SSAP>";
	writeBytes = write (sock, qstrtochar(writebuff), writebuff.length());
	readBytes = read (sock, readb, 300);	
	readb[readBytes] = '\0';
	printf ("Cliente %d: Leimos: %d -%s\n", __LINE__, readBytes, readb);
	readbuff = readb;
	/* deberiamos haber recibido una respuesta ok */
	fail_if (readbuff != "<SSAP>ok<SSAP>");
	
	/* cerramos la conexion */
	close (sock);
}

static void check_send_user ()
{
	int writeBytes = 0;
	int readBytes = 0;
	char readb[300];
	
	
	connect_server (1234, 1238);
	writebuff = "<SSAP>registrar<SSAP>";
	writeBytes = write (sock, qstrtochar(writebuff), writebuff.length());
	readBytes = read (sock, readb, 300);
	readb[readBytes] = '\0';
	printf ("Cliente %d: Leimos: %d -%s\n", __LINE__, readBytes, readb);
	readbuff = readb;
	/* deberiamos haber recibido una respuesta ok */
	fail_if (readbuff != "<SSAP>ok<SSAP>");
	
	writebuff = "<SSAP>av:de:as:tr:01:DF,sapepe<SSAP>";
	printf ("Cliente: Enviando un usuario\n");
	writeBytes = write (sock, qstrtochar(writebuff), writebuff.length());
	printf ("Cliente: Enviamos %d\n", writeBytes);
	readBytes = read (sock, readb, 300);
	readb[readBytes] = '\0';
	printf ("Cliente %d: Leimos: %d -%s\n", __LINE__, readBytes, readb);
	readbuff = readb;
	/* deberiamos haber recibido una respuesta ok */
	fail_if (readbuff != "<SSAP>ok<SSAP>");
	
	writebuff = "<SSAP>av:de:as:tr:01:DF,sapepe<SSAP>";
	printf ("Cliente: Enviando EL MISMO USUARIO\n");
	writeBytes = write (sock, qstrtochar(writebuff), writebuff.length());
	readBytes = read (sock, readb, 300);
	readb[readBytes] = '\0';
	printf ("Cliente %d: Leimos: %d -%s\n", __LINE__, readBytes, readb);
	readbuff = readb;
	/* deberiamos haber recibido una respuesta ok */
	fail_if (readbuff != "<SSAP>ok<SSAP>");
	
	writebuff = "<SSAP>av:de:as:tr:01:DFs,sapepe<SSAP>";
	printf ("Cliente: Enviando un usuario MALFORMADO\n");
	writeBytes = write (sock, qstrtochar(writebuff), writebuff.length());
	readBytes = read (sock, readb, 300);
	printf ("Cliente %d: Leimos: %d\n", __LINE__, readBytes);
	/* deberiamos no poder leer ni bosta */
	fail_unless (readBytes <= 0);
	
	/* cerramos la conexion */
	close (sock);
	
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
	/*fail_if(db->loadFromFile ("pruebaextra.txt") == false);*/
	
	server = new UServer (db, 1234, 1238);
	server->start();
	server->wait(2000);
	
	check_register ();
	check_send_user();
	server->stop();
	server->wait(2000);
	/* Saliendo */
	
	printf ("*****************************************\n");
	db->print();
	printf ("*****************************************\n");
	printf ("Saliendo\n");
	
	
	
	delete db;
	delete user;
	delete server;
	
	
	
	return 0;
}