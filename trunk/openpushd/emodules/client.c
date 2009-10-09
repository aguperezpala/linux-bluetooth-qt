/* Librerías estándar */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
/* Librerías de errores */
#include <assert.h>
#include <errno.h>
/* Librerías de redes */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/* Librerías propias */
#include "client.h"


#define  IPPROTO  0	/* IP protocol number (see /etc/protocols) */

#ifdef _DEBUG
  #ifndef ASSERT
    #define ASSERT(x) assert(x)
  #endif
#else
  #define ASSERT(x)
#endif

struct _client {
	int socket;
	bstring inBuffer;
	bstring outBuffer;
}

/** ~~~~~~~~~~~~~~~~~~~~~~~ Funciones privadas ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ **/













/** ~~~~~~~~~~~~~~~~~~~~~~~ Funciones públicas ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ **/

/* Creador
 * CALL: cli = client_create()
 * POS: cli != NULL
 */
client *create_client (void)
{
	client *cli = NULL;
	
	cli = (client *) calloc (1, sizeof (struct _client));
	ASSERT (cli != NULL);
	
	cli->socket    = -1;
	cli->inBuffer  = bfromcstr("");
	cli->outBuffer = bfromcstr("");
	
	return cli;
}


/* Genera una conexión de tipo cliente con la dirección de internet (ip)
 * a través del puerto (port) para el cliente (cli)
 *
 * PRE: cl != NULL
 *	ip != NULL
 *	port > 0
 *
 * CALL: connected = client_connect (cli, ip, port)
 *
 * POS:  connected && "Conexión establecida y  activa"
 *	  OR
 *	!connected && "No pudo establecerse la conexión"
 */
bool client_connect (client *cli, const char *ip, short port)
{
	bool connected = false;
	struct sockaddr_in addr;
	int err = 0, sock = -1;
	
	ASSERT (cli != NULL);
	ASSERT (ip != NULL);
	ASSERT (port > 0);
	
	
	/* Todo esto es para luego conectarnos */
	addr.sin_family = AF_INET;
	addr.sin_port   = (unsigned short) htons (port);
	err = inet_aton (ip, &addr.sin_addr);
	if (err) {
		perror ("client_connection: IP given is incorrect\n");
		return connected;
	}
	memset (&addr.sin_zero, '\0', 8);
	
	/* Sólo servicio orientado a la conexión con protocolo de red IPv4 */
	sock = socket (PF_INET, SOCK_STREAM, IPPROTO);
	if (sock < 0) {
		perror ("client_connection: Problems creating socket\n");
		return connected;
	}
	
	/* Al fin, conectamos */
	err = connect (sock, (const struct sockaddr *) &addr,
		       (socklen_t) sizeof (struct sockaddr_in));
	if (err) {
		perror ("client_connection: couldn't connect to server\n");
		close (sock);
	} else
		connected = true;
		
		
	return connected;
}


/* Indica si el cliente tiene conexión activa
 *
 * PRE: cli != NULL
 *
 * CALL: connected = client_is_connected (cli)
 *
 * POS:  connected && "Sí está conectado"
 *	  OR
 *	!connected && "No está conectado"
 */
bool client_is_connected (client *cli)
{
	ASSERT (cli != NULL);
	return cli->socket > 0;
}