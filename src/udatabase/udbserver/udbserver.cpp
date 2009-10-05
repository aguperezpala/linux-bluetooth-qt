#include "udbserver.h"


/* Definimos la lista de funciones RPC, el nombre indica claramente que lo que
* deberia hacer la funcion. */

int udbs_RPCgetUserFromMac (void * req, void * resp);



/* Funcion que se encarga de chequear si esta todo correctamente cuando
 * intentamos registrar.
 * Requires:
 *	req.isNull () == false
 * RETURNS:
 *	-1 	error de protocolo u otra cosa
 *	0	si esta incompleta
 *	1	si esta completo el request y es "register"
 */
static int udbs_registerChek (QString &req)
{
	int result = 0;
	QString * reqParsed = NULL;
	
	/* pre */
	if (req.isNull() == false) {
		debugp("udbs_registerChek: is NULL\n");
		return -1;
	}
	
	
	/* chequeamos en que estado estamos */
	switch (udbs_isValidRequest(req)) {
		case UDBS_NO_ERROR:
			/* verificamos si es "register" el dato */
			reqParsed = udbs_parseRequest(req);
			if (reqParsed == NULL || *reqParsed != "registrar")
				/*! no respeta el protocolo o algo raro */
				result = -1;
			else {
				/*! es correcto :) y se registro bien */
				result = 1;
				/* liberamos */
				delete reqParsed;
			}
			break;
		case UDBS_INCOMPLETE:
			/* tenemos que seguir recibiendo */
			result = 0;
			break;
		default:
			/* cualquier otra cosa es un erroroncho */
			result = -1;
			break;
	}
	return result;
}
			
/* Funcion que espera que un usuario se registre, chequeando el protocolo
* y asegurandose que todo este "bajo control".
* NOTE: es bloqueante (modificar para que no sea asi?).
* REQUIRES:
*	client != NULL
* RETURNS:
*	< 0 	on error
*	== 0	if no error.
*/
int udbs_registerClient (SClient * client)
{
	int result = -1;
	QString req = "";
	char buff[SSDBP_MAX_BUFF_SIZE+1] = {0};
	int bytesReaded = 0;
	
	
	/* pre */
	if (client == NULL) {
		ASSERT (false);	/* debug */
		return result;
	}
	
	result = 0;
	/* limpiamos un cacho */
	client->clearBuffer();
	/* primero que todo vamos a cumplir 1) */
	while (result == 0) {
		/* leemos */
		bytesReaded = client->continueReading();
		if (bytesReaded <= 0) {
			debugp("udbs_registerClient: Error al intentar leer "
				"el cliente. No se pudo registrar ademas.\n");
			result = -1;
			continue;
		}
		/* verificamos lo que leimos */
		strncpy (buff, client->getData(), client->getDataSize());
		buff[client->getDataSize()] = '\0';
		req = buff;
		/* verificamos que este completo con el parser */
		result = udbs_registerChek (req);
	}
	
	/*! aca solamente verificamos que result == 1 ==> que se registro
	 * correctamente, caso contrario va a ser result = -1 */
	if (result == 1) {
		QString resp = "ok";
		
		/* Le respondemos un "ok" */
		result = 0;
		udbs_generateResponse (resp);
		if (client->sendData(qstrtochar(resp), resp.size()) <= 0)
			/* fucking error enviando :( */
			result = -1;
	} else
		ASSERT (result == -1);	/* al pedo pero bue */
	
	return result;
}


/* Funcion que espera recibir una cadena sin errores y completa de un cliente
* determinado, para que cuando se obtenga el request del cliente, sea un
* request "correcto" en cuanto a protocolo.
* NOTE: es bloqueante (modificar dividiendola en 2 partes a la funcion:
* Ya que va a tener que devolver "incomplete_info" en caso de no haber recibido
* todos los datos).
* REQUIRES:
*	client != NULL
* RETURNS:
*	< 0 	on error
*	0 	if NO error (NOTE: ENSURES: udbs_isValidRequest (client->data) 
*				  == UDBS_NO_ERROR)
*/
int udbs_receiveReq (SClient * client)
{
	QString req = "";
	QString resp = "";
	char buff[SSDBP_MAX_BUFF_SIZE+1] = {0};
	int bytesReaded = 0;
	int result = 0;
	bool notRecived = true;
	
	req = "";
	resp = "";
	memset (buff, '\0', USP_SSAP_MAZ_SIZE);
	client->clearBuffer();
	
	while (result == 0 && notRecived) {
	/* leemos */
		bytesReaded = client->continueReading();
		if (bytesReaded <= 0) {
			debugp("udbs_receiveReq: Error al intentar leer "
				"el cliente. No se pudo registrar ademas.\n");
			result = -1;
			continue;
		}
		/* obtenemos los datos */
		strncpy (buff, client->getData(), client->getDataSize());
		buff[client->getDataSize()] = '\0';
		req = buff;
		
		
		/* verificamos que este completo */
		switch (udbs_isValidRequest (req)) {
			case UDBS_NO_ERROR: 
				/* no hay error=> listo el poio */
				result = 0;
				notRecived = false;
					break;
					
			case UDBS_INCOMPLETE:
				/* no hacemos nada, seguimos recibiendo */
				break;
				
			default:
				/* cualquier otro caso es error */
				result = -1;
			}
			
	}
	
	/* le respondemos un "ok" respetando el protocolo SI NO HUBO ERROR */
	if (!notRecived && result == 0) {
		QString resp = "ok";
		/* Le respondemos un "ok" */
		udbs_generateResponse (resp);
		if (client->sendData(qstrtochar(resp), resp.size()) <= 0)
			/* fucking error enviando :( */
			result = -1;
	}

	return result;
}

/*! funcion super compleja, Se encarga de trabajar las RPC, deberiamos hacer
* una arreglo de tuplas (string, func pointer) para determinar cada una
* de las RPC
*/
/* Funcion que va a ejecutar una RPC de un cliente, y a su vez responderle
* con los datos nescesarios. Es una funcion compleja.
* REQUIRES:
*	client != NULL
*	is_validRequest (client->data)
*	udb 	!= NULL
* RETURNS:
*	-1	si hubo un pedido fallido (no respeta el protocolo. NOTE: NO se
*		cierra la conexion, ni se libera el client, hacerlo de arriba).
*	-2	si hubo un error interno...
*	== 0 	NO hubo error
*/
int udbs_RPCWork (SClient * client, const UDataBase * udb)
{
	



