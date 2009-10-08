/* Librerías estándar */
#include <stdio.h>
#include <stdbool.h>
/* Librerías de errores */
#include <assert.h>
#include <errno.h>
/* Librerías de redes */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#define  IPPROTO  0	/* IP protocol number (see /etc/protocols) */



/** ~~~~~~~~~~~~~~~~~~~~~~~ Funciones privadas ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ **/




/* Genera una conexión de tipo cliente con la dirección de internet (ip)
 * a través del puerto (port)
 * Devuelve el file descriptor de la conexión activa.
 * NOTE: se *genera* una conexión, no solo un socket (se usa connect)
 *
 * PRE: ip != NULL && port > 0
 *
 *    socket = setup_client_connection (ip, port)
 *
 * POS: socket  >  0 && "Conexión establecida y  activa"  ||
 *	socket == -1 && "No pudo establecerse la conexión"
 */
static int setup_client_connection (const char *ip, short port)
{
	struct sockaddr_in addr;
	int err = 0, sock = -1;
	
	assert (ip != NULL);
	assert (port > 0);
	
	
	addr.sin_family = AF_INET;
	addr.sin_port   = (unsigned short) htons (port);
	err = inet_aton (ip, addr.sin_addr);
	if (err) {
		perror ("client_connection: IP given is incorrect\n");
		return sock;
	}
	memset (&addr.sin_zero, '\0', 8);
	
	
	/* Sólo servicio orientado a la conexión con protocolo de red IPv4 */
	sock = socket (PF_INET, SOCK_STREAM, IPPROTO);
	if (sock < 0) {
		perror ("client_connection: Problems creating socket\n");
		return sock;
	}
	
	
	err = connect (sock, (const struct sockaddr *) &addr,
		        (socklen_t) sizeof (struct sockaddr_in));
	if (err) {
		perror ("client_connection: couldn't connect to server\n");
		close (sock);
		return -1;
	}
		
	return sock;
}









/** ~~~~~~~~~~~~~~~~~~~~~~~ Funciones públicas ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ **/


Cuac
