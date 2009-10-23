#include "ObexDBC.h"

#define ODBCON_REGS_CMD		"<SSDBP>registrar<SSDBP>"
#define ODBCON_OK_RESP		"<SSDBP>ok<SSDBP>"

struct _oDBCon {
	client_t * client;	/* el cliente de carlox */
	pthread_mutex_t mutex;	/* concurrente :) */
	short port;		/* guardamos por las dudas el puerto */
	char ip[16];		/* vamos a guardar por las dudas el ip */
};


/* Funcion que se conecta y registra automaticamente.
 * REQUIRES:
 *	odbc != NULL
 * RETURNS:
 *	true	if succes
 *	false	otherwise
 */
static bool odbc_connect_and_login (oDBCon_t * odbc)
{
	bool result = false;
	int bSended = 0;
	char buff[50] = {0};	/*! tener en cuenta que la resp sea menor */
	
	if (!odbc)
		return false;
	
	/* nos vamos a intentar desconectar... */
	if (client_is_connected(odbc->client))
		client_disconnect(odbc->client);
	
	/* nos vamos a conectar y loguear */
	result = client_connect(odbc->client, odbc->ip, odbc->port);
	if (!result)
		return false;
	
	/* ahora nos vamos a loguear */
	bSended = client_send(odbc->client, ODBCON_REGS_CMD, 
			       strlen(ODBCON_REGS_CMD));
	if (bSended <= 0)
		return false;
	/* si nos pudemis conectar y mandar => intentamos ahora recibir el ok
	 */
	bSended = client_receive(odbc->client, buff, 50);
	/* ahora chequeamos lo que recibimos */
	if ((bSended <= 0) || (strlen (ODBCON_OK_RESP) != bSended) ||
		strcmp(ODBCON_OK_RESP, buff) != 0) {
		fprintf (stderr, "Recibimos una respuesta distinta de "
		"ODBCON_OK_RESP\n");
		return false;
	}
	/* ta todo joya => tamos registrados y conectados */
	
	return true;
}

/* Constructor */
oDBCon_t * odbc_create (void)
{
	oDBCon_t * result = NULL;
	
	result = calloc (1, sizeof (*result));
	if (result) {
		result->client = create_client ();
		pthread_mutex_init (&result->mutex, NULL);
	
	return result;
}

/* destructor.
* REQUIRES:
* 	odbc != NULL
*/
void odbc_destroy (oDBCon_t * obdc)
{
	if (!obdc)
		return;
	if (odbc->client)
		client_destroy(odbc->client);
	
	pthread_mutex_destroy(&odbc->mutex);
	free (odbc);
}

/* Funcion que establece la conexion con la base de datos, y se registra.
* REQUIRES:
*	odbc 	!= NULL
*	ip	!= NULL
*	port	> 0
* RETURNS:
*	0	if success
*	errCode on error
*/
int odbc_connect (oDBCon_t * obdc, const char * ip, short port)
{
	int status = -1;
	
	if (!odbc)
		return -1;
	
	/* vamos a guardar la ip y el puerto por las dudas que tengamos que
	 * hacer una reconexion luego */
	odbc->port = port;
	strncpy(odbc->ip, ip, sizeof(odbc->ip));
	
	/*! Atomic */
	pthread_mutex_lock(&odbc->mutex);
	
	if (odbc_connect_and_login(odbc))
		status = 0;
	
	pthread_mutex_unlock(&odbc->mutex);
	
	return status;
}

/* Funcion que sirve para desconectarse de la base de datos.
* REQUIRES:
*	odbc 	!= NULL
*/
void odbc_disconnect (oDBCon_t * obdc)
{
	if (!odbc)
		return;
	
	if (client_is_connected(odbc->client))
		client_disconnect(odbc->client);
}
	
	

/* Funcion que chequea si estamos conectados o no a la base de datos
* REQUIRES:
*	odbc != NULL
* RETURNS:
*	true 	if we are connected
*	false	otherwise
*/
bool odbc_is_connected (oDBCon_t * obdc)
{
	if (!odbc)
		return false;
	
	return client_is_connected(odbc->client);
}

/* Se busca en la base de datos la dirección MAC pasada como parámetro
* Si la misma está registrada. De ser así se retorna el nombre de usuario
* asociado. Sino se devuelve NULL.
*
* PRE: MAC 	!= NULL
*	odbc 	!= NULL
*	odbc_is_connected(odbc);
*
* CALL: usr = existUser (MAC)
*
* POS: usr != NULL && "El nombre de usuario asociado a MAC es 'usr'"
*	  OR
*	usr == NULL &&  ("No hay conexión con la BD"
*			   OR
*			 "El cliente no está registrado en la BD")
*//*! NOTE: ATOMIC */
char *odbc_existUser (oDBCon_t * odbc, const char *MAC)
{
	char * result = NULL;
	bool isConnected = true;
	int bytes = 0;
	
	if (!odbc)
		return NULL;
	
	/*! Atomic */
	pthread_mutex_lock(&odbc->mutex);
	/* verificamos que estemos conectados, si no, intentamos reconectarnos */
	if (!client_is_connected(odbc->client))
		isConnected = odbc_connect_and_login(odbc);
	
	pthread_mutex_unlock(&odbc->mutex);
	
	if (!isConnected) {
		fprintf (stderr, "Error al intentar enviar datos, no estamos "
		"conectados a la base de datos\n");
		return NULL;
	}
	
	/*! la vamos hacer atomica */
	pthread_mutex_lock(&odbc->mutex);
	/*!FIXME: aca deberiamos mandar enprotocolados los datos, no asi nomas
	*/
	if ((bytes = client_send(odbc->client, MAC, strlen(MAC))) <= 0)
		/*! error cacaso */
		return NULL;
	/* ahora vamos a recibir si y solo si pudimos mandar todo

/* Funcion que va a agregar un usuario a la base de datos.
* REQUIRES:
*	MAC 	!= NULL
*	nick	!= NULL
*	odbc 	!= NULL
*	odbc_is_connected(odbc);
* RETURNS:
*	true	on success
*	false	otherwise
*//*! NOTE: ATOMIC */
bool odbc_addUser (oDBCon_t * odbc, const char * MAC, const char * nick);
