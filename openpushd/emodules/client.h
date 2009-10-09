#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H



/* Genera una conexión de tipo cliente con la dirección de internet (ip)
 * a través del puerto (port)
 * Devuelve el file descriptor de la conexión activa.
 * NOTE: se *genera* una conexión, no solo un socket (se usa connect)
 *
 * PRE: ip != NULL && port > 0
 *
 *    socket = connect_client (ip, port)
 *
 * POS: socket  >  0 && "Conexión establecida y  activa"  ||
 *	socket == -1 && "No pudo establecerse la conexión"
 */
int connect_client (const char *ip, short port)



#endif
