#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H


typedef struct _client client;



/* Creador
 * CALL: cli = client_create()
 * POS: cli != NULL
 */
client *create_client (void);


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
bool client_connect (client *cli, const char *ip, short port);


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
bool client_is_connected (client *cli);



int disconnect_client (void);


#endif
