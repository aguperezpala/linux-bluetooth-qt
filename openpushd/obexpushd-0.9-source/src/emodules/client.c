#define _GNU_SOURCE		/* para strnlen */

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

#ifdef __DEBUG
  #ifndef ASSERT
    #define ASSERT(x) assert(x)
  #endif
#else
  #define ASSERT(x)
#endif

struct _client {
	int socket;
/*	bstring inBuffer;
	bstring outBuffer;*/
};


/** ~~~~~~~~~~~~~~~~~~~~~~~ Funciones privadas ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ **/



/* ... contate algo che */




/** ~~~~~~~~~~~~~~~~~~~~~~~ Funciones públicas ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ **/



/* Creador
 * CALL: cli = client_create()
 * POS: cli != NULL
 */
client_t *create_client (void)
{
	client_t *cli = NULL;
	
	cli = (client *) calloc (1, sizeof (struct _client));
	ASSERT (cli != NULL);
	
	cli->socket    = -1;
	
	return cli;
}



/* Destructor
 * PRE: cli != NULL
 * CALL: cli = client_destroy (cli)
 * POS: cli == NULL && "memoria liberada"
 */
client_t *client_destroy (client_t *cli)
{
	ASSERT (cli != NULL);
	
	free (cli);
	
	return NULL;
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
bool client_is_connected (client_t *cli)
{
	ASSERT (cli != NULL);
	return cli->socket > 0;
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
bool client_connect (client_t *cli, const char *ip, short port)
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
		perror ("client_connect: IP given is incorrect\n");
		return connected;
	}
	memset (&addr.sin_zero, '\0', 8);
	
	/* Sólo servicio orientado a la conexión con protocolo de red IPv4 */
	sock = socket (PF_INET, SOCK_STREAM, IPPROTO);
	if (sock < 0) {
		perror ("client_connect: Problems creating socket\n");
		return connected;
	}
	
	/* Al fin, conectamos */
	err = connect (sock, (const struct sockaddr *) &addr,
		       (socklen_t) sizeof (struct sockaddr_in));
	if (err) {
		perror ("client_connect: couldn't connect to server\n");
		close (sock);
	} else
		connected = true;
		
		
	return connected;
}



/* Desconecta al cliente así de pecho
 *
 * PRE: cli != NULL
 *	client_is_connected (cli)
 *
 * CALL: disconnect_client (cli)
 *
 * POS: !client_is_connected (cli)
 */
void client_disconnect (client_t *cli)
{
	ASSERT (cli != NULL);
	
	close (cli->socket);
	cli->socket = -1;
	
	return;
}



/* Envía un mensaje (msg) de longitud (len) por la conexión del cliente (cli)
 * Es bloqueante hasta haber enviado todo, o hasta que ocurra un error
 *
 * PRE: cli != NULL
 *	client_is_connected (cli)
 *
 * CALL: count = client_send (cli, msg, len)
 *
 * POS: count > 0 && "se enviaron (count) caracteres del mensaje"
 *	  OR
 *	count = 0 && "no se envió nada, mensaje inválido"
 *	  OR
 *	count < 0 && "error durante el envío, se pudieron enviar (count) bytes"
 */
int client_send (client_t *cli, const char *msg, size_t len)
{
	int count = 0, sent = 0;
	
	ASSERT (cli != NULL);
	ASSERT (client_is_connected (cli));
	
	if ( !( strnlen(msg,1) <= 0 ) )
		return count;
	
	while (-1 < count && (size_t) count < len) {
		/* Enviamos todo, ó hasta que salte un error */
		sent = send (cli->socket, msg+count, len-count, MSG_WAITALL);
	"compilá esto puto"
		if (sent > 0)
			count += sent;
		else
			count = (-1) * count;
	}
	
#ifdef __DEBUG
	if (count < 0)
		perror("client_send: couldn't send data\n");
#endif
	
	return count;
}


/* Recibe un mensaje por la conexión del cliente (cli). El buffer (msg) ya debe
 * poseer el tamaño de memoria especificado en (len)
 *
 * PRE: cli != NULL
 *	client_is_connected (cli)
 *
 * CALL: count = client_receive (cli, msg, len)
 *
 * POS: count > 0 && "se recibieron (count) caracteres"
 *	  OR
 *	count < 0 && "error durante la recepción, se recibieron -count bytes"
 */
int client_receive (client_t *cli, char *msg, size_t len)
{
	int count = 0, recvd = 0;
	
	ASSERT (cli != NULL);
	ASSERT (client_is_connected (cli));
	
	while (-1 < count && (size_t) count < len) {
		/* Recibimos todo, ó hasta que salte un error */
		recvd = recv (cli->socket, msg+count, len-count, MSG_WAITALL);
		if (recvd > 0)
			count += recvd;
		else
			count = (-1) * count;
	}
	
#ifdef __DEBUG
	if (count < 0)
		perror("client_send: couldn't receive data\n");
#endif
	
	return count;
}
	
	
/* Recibe un mensaje */