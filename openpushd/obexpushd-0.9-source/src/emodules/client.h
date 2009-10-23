#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H


typedef struct _client client_t;




/* Creador
 * CALL: cli = client_create()
 * POS: cli != NULL
 */
client_t *create_client (void);


/* Destructor
 * PRE: cli != NULL
 * CALL: cli = client_destroy (cli)
 * POS: cli == NULL && "memoria liberada"
 */
client_t *client_destroy (client_t *cli);


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
bool client_is_connected (client_t *cli);


/* Genera una conexión de tipo cliente con la dirección de internet (ip)
 * a través del puerto (port) para el cliente (cli)
 *
 * PRE: cl != NULL
 *	ip != NULL
 *	port > 0
 *	!client_is_connected (cli)
 *
 * CALL: connected = client_connect (cli, ip, port)
 *
 * POS:  connected && "Conexión establecida y  activa"
 *	  OR
 *	!connected && "No pudo establecerse la conexión"
 */
bool client_connect (client_t *cli, const char *ip, short port);


/* Desconecta al cliente así de pecho
 *
 * PRE: cli != NULL
 *	client_is_connected (cli)
 *
 * CALL: disconnect_client (cli)
 *
 * POS: !client_is_connected (cli)
 */
void client_disconnect (client_t *cli);


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
 *	count < 0 && "no se pudo enviar, error de envío"
 */
int client_send (client_t *cli, const char *msg, size_t len);


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
int client_receive (client_t *cli, char *msg, size_t len);


#endif
