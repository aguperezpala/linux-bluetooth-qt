#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <QString>
#include "sserver.h"
#include "sclient.h"
#include "../tester.h"
#include "../consts.h"

SServer * server;
SClient * cl1;
int sock;
char buffSend[200];
char buffRecv[200];
int bs, br;
pthread_t thread1;

void * send_thread (void* s)
{
	s = NULL;
	bs = send (sock, buffSend, strlen (buffSend),0);
	
	return NULL;
}

void * recv_thread (void* s)
{
	s = NULL;
	int size = (int)strlen(buffSend);
	br = 0;
	printf ("*******size(buffSend):%d\n", size);
	while (br < size ) {
		br += recv(sock, buffRecv, 200, 0);
		printf ("recibimos: br:%d\n", br);
	}
	
	return NULL;
}
	
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

static void check_recv (void)
{
	int size = 0;
	/* enviamos datos */
	
	strcpy (buffSend ,"esto es una prueba");
	size = strlen (buffSend);
	pthread_create(&thread1, NULL, &send_thread,
			(void *) &size);	
	
	
	/* chequeamos que reciba bien */
	br = cl1->readData ();
	while (br != (int) strlen (buffSend))
		br += cl1->continueReading();
	strncpy (buffRecv,cl1->getData(), br);
	printf ("Recibimos: %s \t%d\n",buffRecv, __LINE__);
	fail_if (strcmp (buffRecv, cl1->getData()) != 0);
	
	cl1->clearBuffer();
	fail_if (cl1->getDataSize () != 0);
	fail_if (strcmp (cl1->getData(), "") != 0);
	
	
	/* esperamos al thread */
	pthread_join(thread1, NULL);
	
}

static void check_send (void)
{
	
	strcpy (buffSend ,"esto es una prueba");
	memset (buffRecv, '\0', 200);
	/* enviamos datos ahora */
	pthread_create(&thread1, NULL, &recv_thread,
			(void *)NULL);
	
	printf ("Estamos recibiendo por recv_thread\n");
	bs = cl1->sendData(buffSend, strlen (buffSend));
	printf ("mandamos: cl1->sendData: size= %d\n",bs);
	pthread_join(thread1, NULL);
	
	printf ("bs: %d\tbr: %d\tline: %d\n", bs, br, __LINE__);
	fail_if (bs != br);
	fail_if (bs != (int)strlen(buffSend));
	buffRecv[strlen(buffSend)] = '\0';
	printf ("Recibimos: %s \t%d\n",buffRecv, __LINE__);
	fail_if (strcmp (buffRecv, buffSend) != 0);
	
}

int main (void)
{
	server = new SServer(100);
	
	
	fail_unless (server->startListen (1234));
	/* nos conectamos al servidor ahora */
	connect_server (1234,1234);
	cl1 = server->acceptClient();
	fail_if (cl1 == NULL);
	
	check_send();
	check_recv();
	
	
	
	return 0;
}