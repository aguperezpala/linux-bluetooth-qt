#include "ObexDBC.h"

#define ODBCON_REGS_CMD		"<SSDBP>registrar<SSDBP>"
#define ODBCON_OK_RESP		"<SSDBP>ok<SSDBP>"
#define ODBCON_FALSE_RESP	"<SSDBP><false><SSDBP>"
#define ODBCON_HEADER		"<SSDBP>"
#define ODBCON_TAIL		"<SSDBP>"
#define ODBCON_SEPARATOR	"<;>"

/*! LAS RPC */
#define ODBCON_RPC_GETUSER	"RPCgetUserFromMac"
#define ODBCON_RPC_ADDUSER	"RPCaddUser"

struct _oDBCon {
	client_t * client;	/* el cliente de carlox */
	pthread_mutex_t mutex;	/* concurrente :) */
	short port;		/* guardamos por las dudas el puerto */
	char ip[16];		/* vamos a guardar por las dudas el ip */
};




/* Funcion que genera las RemoteProcedureCall respetando el protocolo SSDBP.
 * REQUIRES:
 *	funcName != NULL
 *	paramList != NULL && paramList[size] = NULL (termina en null)
 * RETURNS:
 *	protocoledData (RPC)		on success
 *	NULL				on error
 * NOTE: Genera memoria
*/
static char * odbc_make_rpc_protocoled(const char * funcName,
					char ** paramList)
{
	char * result = NULL;
	bool error = false;
	int i = 0;
	/* Pres */
	if (!funcName || !paramList)
		return result;
	
	/* allocamos memoria, 256 bytes. no soporta mas el protocolo */
	result = calloc (257, sizeof(*result));
	if (!result)
		return result;
	/* ahora vamos a ir encolando las cosas */
	strcpy(result, ODBCON_HEADER);
	strcat(result, funcName);
	
	/* ahora encolamos los parametros y los separadores */
	while (!error && paramList[i] != NULL) {
		if (strlen(paramList[i]) + strlen(result) > 256) {
			error = true;
			continue;
		}
		strcat(result, ODBCON_SEPARATOR);
		strcat(result, paramList[i]);
		i++;
	}
	/* verificamos si hubo errror, devolvemos null */
	if (error) {
		free(result);
		result = NULL;
	}
	
	return result;
}
	



/* Funcion que se va a encargar de recibir datos y recibir hasta obtener
 * un header y un tail determinado, o hasta exeder de recibir n bytes.
 * Va a devolver los datos recibidos.
 * REQUIRES:
 *	client != NULL
 *	recvSize > 0
 * RETURNS:
 *	received_data	on success 
 *	NULL		on error
 * NOTE: Si header || tail == null entonces no chequeamos esto.
 * NOTE: genera memoria
 */
static char * odbc_receive_data(client_t * client, char * header, char * tail,
				 int recvSize)
{
	char * result = NULL;
	bool error = false, done = false;
	bool checkHeader = true, checkTail = true;
	int bytesRecv = 0, actualRcvB = 0;
	
	
	/* PRES */
	if (!client || recvSize <= 0)
		return NULL;
	
	/* que debemos chequear */
	if (!header)
		checkHeader = false;
	if (!tail)
		checkTail = false;
	
	/* allocamos memoria, despues reduciremos. */
	result = calloc(recvSize, sizeof(*result));
	if (!result)
		return result;
	/* ahora empezamos a leer mientras estemos dentro del rango de datos
	 * a recibir. */
	while (!error && bytesRecv <= recvSize && !done) {
		actualRcvB = client_receive(client, &result[bytesRecv],
					     recvSize - bytesRecv);
		/* verificamos si hay error */
		if (actualRcvB <= 0) {
			error = true;
			continue;
		}
		bytesRecv += actualRcvB;
		/* verificamos ahora si lo que estamos recibiendo corresponde
		 * a lo que queremos realmente recibir */
		if (checkHeader) {
			error = ((strstr(result, header) == NULL) &&
				(int)strlen(header) <= bytesRecv) ||
				/*! si encotramos el header no al principio =>
				 * es un error. */
				(strstr(result, header) != result);
			/* ahora vemos, si no hubo error y existe el header =>
			 * no lo chequeamos mas */
			checkHeader = !error && (strstr(result, header) != NULL);
		}
		/* si hubo error chau */
		if (error)
			continue;
		/* verificamos el tail ahora... */
		if (checkTail) {
			error = (strstr(result, tail) == NULL) && 
				bytesRecv >= recvSize;
			/* seguimos verificando el tail? si no, entonces es
			 * porque ya estamos :D */
			done = (strstr(result, tail) != NULL);
		}
		if (error)
			continue;
	}
	/* si hubo errores => borramos y devolvemos null */
	if (error && !done) {
		free(result);
		result = NULL;
	} else {
		/* no hubo error, reallocamos entonces result al tamaño
		 * adecuado */
		result = realloc(result, bytesRecv + 1);
	}
	
	return result;
}
	

 
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
	char * response = NULL;
	
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
	response = odbc_receive_data(odbc->client, ODBCON_HEADER, ODBCON_TAIL,
				      strlen(ODBCON_HEADER) + 
				      strlen(ODBCON_TAIL) + 5);
	/* verificamos que el response sea el correcto */
	if (response)
		if (strcmp(ODBCON_OK_RESP, response) == 0)
			free(response);
		else {
			free(response);
			return false; /*! error */
		}
	else
		return false; /*! error */
		
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
	}
	
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
	if (obdc->client)
		client_destroy(obdc->client);
	
	pthread_mutex_destroy(&obdc->mutex);
	free (obdc);
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
int odbc_connect (oDBCon_t * odbc, const char * ip, short port)
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
void odbc_disconnect (oDBCon_t * odbc)
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
bool odbc_is_connected (oDBCon_t * odbc)
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
	bool isConnected = true;
	int bytes = 0;
	char ** params = NULL;
	char * response = NULL;
	char * request = NULL;
	
	if (!odbc || !MAC)
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
	
	/* generamos memoria para los parametros y esas cosas */
	params = (char **) calloc(2, sizeof(*params));
	if (!params)
		return NULL;
	/* allocamos para el primer parametro */
	params[0] = calloc(strlen(MAC) + 1, sizeof(*params[0]));
	if (!params[0]) {
		free(params);
		return NULL;
	}
	/* copiamos los parametros */
	strcpy(params[0], MAC);
	request = odbc_make_rpc_protocoled(ODBCON_RPC_GETUSER, params);
	free(params[0]); free(params);
	/* si no podemos formar el request morimos */
	if (!request)
		return NULL;
	
	/*! la vamos hacer atomica */
	pthread_mutex_lock(&odbc->mutex);
	
	if ((bytes = client_send(odbc->client, request, strlen(request))) <= 0){
		pthread_mutex_unlock(&odbc->mutex); /*! liberamos el candado */
		free (request);
		/*! error cacaso */
		return NULL;
	}
	
	free(request); request = NULL;
	/* ahora vamos a recibir <==> pudimos mandar todo >>> 70 = nickSize */
	response = odbc_receive_data(odbc->client, ODBCON_HEADER, ODBCON_TAIL,
						 strlen(ODBCON_HEADER) + 
						 strlen(ODBCON_TAIL) + 70);
	
	pthread_mutex_unlock(&odbc->mutex); /*! liberamos el candado */
	if (!response)
		return NULL;
	/* si tenemos un response vamos a ver 2 cosas... si es <false> devolvemo
	 * null */
	if (strcmp(response, ODBCON_FALSE_RESP) == 0) {
		free(response); response = NULL;
	}
	
	return response;
}

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
bool odbc_addUser (oDBCon_t * odbc, const char * MAC, const char * nick)
{
	bool result = false;
	bool isConnected = true;
	int bytes = 0;
	char ** params = NULL;
	char * response = NULL;
	char * request = NULL;
	
	if (!odbc || !MAC || !nick)
		return false;
	
	/*! Atomic */
	pthread_mutex_lock(&odbc->mutex);
	/* verificamos que estemos conectados, si no, intentamos reconectarnos */
	if (!client_is_connected(odbc->client))
		isConnected = odbc_connect_and_login(odbc);
	
	pthread_mutex_unlock(&odbc->mutex);
	
	if (!isConnected) {
		fprintf (stderr, "Error al intentar enviar datos, no estamos "
		"conectados a la base de datos\n");
		return false;
	}
	
	/* generamos memoria para los parametros y esas cosas */
	params = (char **) calloc(3, sizeof(*params));
	if (!params)
		return false;
	/* allocamos para el primer parametro */
	params[0] = calloc(strlen(MAC) + 1, sizeof(*params[0]));
	if (!params[0]) {
		free(params);
		return false;
	}
	params[1] = calloc(strlen(nick) + 1, sizeof(*params[1]));
	if (!params[1]) {
		free(params[0]);
		free(params);
		return false;
	}
	/* copiamos los parametros */
	strcpy(params[0], MAC);
	strcpy(params[1], nick);
	request = odbc_make_rpc_protocoled(ODBCON_RPC_GETUSER, params);
	free(params[0]); free(params[1]); free(params);
	/* si no podemos formar el request morimos */
	if (!request)
		return false;
	
	/*! la vamos hacer atomica */
	pthread_mutex_lock(&odbc->mutex);
	
	if ((bytes = client_send(odbc->client, request, strlen(request))) <= 0){
		free (request);
		/*! error cacaso */
		return false;
	}
	
	free(request); request = NULL;
	/* ahora vamos a recibir <==> pudimos mandar todo >>> 70 = nickSize */
	response = odbc_receive_data(odbc->client, ODBCON_HEADER, ODBCON_TAIL,
						 strlen(ODBCON_HEADER) + 
						 strlen(ODBCON_TAIL) + 10);
	if (!response)
		return false;
	/* si tenemos un response vamos a ver 2 cosas... si es <false> devolvemo
	 * null */
	if (strcmp(response, ODBCON_OK_RESP) == 0)
		result = true;

	free(response); response = NULL;
	
	return result;
}

