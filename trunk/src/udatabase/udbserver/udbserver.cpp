#include "udbserver.h"



/*! Ahora vamos a definir un arreglo de tuplas para determinar las funciones
* y los punteros a las funciones correspondientes. El arreglo tiene que
* si o si terminar en NULL, para saber en donde termina.
*/
rpcTuple_t RPC_FUNCS[2] = {
	{"getUserFromMac", udbs_RPCgetUserFromMac},
	{"RPCaddUser", udbs_RPCaddUser},
	{NULL, NULL}
};


/*! Funcion que sirve para desglosar un pedido del tipo 
 *  funcName<;>param1<;>param2...paramN. Devolviendo una QStringList donde el
 * primer valor es el nombre de la funcion (funcName), y los siguientes valores
 *  son los diferentes parametros.
 * REQUIRES;
 *	req.isNull () == false
 *	req 	ya fue parseada (se le sacaron los headers)
 * RETURNS:
 *	QStringList 	!= NULL (NOTE: Allocamos memoria)
 *	NULL		si hay error 
 */
static QStringList * parseRPCtoList (QString & req)
{
	QStringList * list = NULL;
	
	/* pre */
	if (req.isNull() == true) {
		debugp ("parseRPCtoList: req null\n");
		ASSERT (false);
		return NULL;
	}
	/* creamos la lista */
	list = new QStringList(req.split (RPC_PARAM_DIVISOR));
	
	return list;
}


/* Definimos la lista de funciones RPC, el nombre indica claramente que lo que
* deberia hacer la funcion. */

/* Esta funcion va a preguntar a la base de datos si existe un usuario
 * determinado segun la mac:
 * REQUIRES:
 *	db != NULL
 *	paramList != NULL
 *	resp != NULL && resp.isNull() == true
 * RETURNS:
 *	< 0	error (de protocolo o lo que sea)
 *	== 0	si no hubo error & resp.isNull() == false
 */
int udbs_RPCgetUserFromMac (UDataBase * db, void * paramList, void * rsp)
{
	CUser * user = NULL;
	QStringList * list = (QStringList *) paramList;
	QString req = "";
	QString * resp = (QString *)rsp;
	
	/* pres */
	if (db == NULL || list == NULL) {
		debugp ("udbs_RPCgetUserFromMac: db NULL\n");
		ASSERT (false);
		return -1;
	}
	/* obtenemos el primer parametro (NOTE: unico que requiere esto) */
	if (list->size() != 1) /* no respeta el protocolo */
		return -1;
	
	req = list->front();
	if (req.isNull() || req.size() != MAC_SIZE) {
		debugp ("udbs_RPCgetUserFromMac: req problem\n");
		ASSERT (false);
		return -1;
	}
	/* pres */
	if (resp == NULL || resp->isNull() == false) {
		debugp ("udbs_RPCgetUserFromMac: resp problem\n");
		ASSERT (false);
		return -1;
	}
	
	/* aca ahora lo que vamos hacer es simplemente llamar a la funcion de
	 * la base de datos pidiendo por el usuario */
	user = ((UDataBase*)db)->getUserFromMac (req);
	if (user == NULL)
		/* quiere decir que no existe ni chota */
		*resp = PRO_RESP_FALSE;
	else {
		/* es porque tenemos un usuario => obtenemos el nick, ya que
		 * esto va a ser la respuesta */
		*resp = user->getNick();
		/* lo eliminamos */
		delete user;
	}
	
	return 0;
	
}


/* Funcion que agrega a la base de datos un determinado usuario.
* REQUIRES:
*	db != NULL
*	paramList != NULL
*	resp != NULL && resp.isNull() == true
* RETURNS:
*	< 0	error (de protocolo o lo que sea)
*	== 0	si no hubo error & resp.isNull() == false
*/
int udbs_RPCaddUser (UDataBase * db, void * paramList, void * rsp)
{
	CUser * user = NULL;
	QStringList * list = (QStringList *) paramList;
	QString mac = "", nick = "";
	QString * resp = (QString *)rsp;
	
	/* pres */
	if (db == NULL || list == NULL) {
		debugp ("udbs_RPCaddUser: db NULL\n");
		ASSERT (false);
		return -1;
	}
	/* obtenemos los 2 parametros (MAC & Nickname) */
	if (list->size() != 2) /* no respeta el protocolo */
		return -1;
	
	mac = list->front();
	if (mac.isNull() || mac.size() != MAC_SIZE) {
		debugp ("udbs_RPCaddUser: mac problem\n");
		ASSERT (false);
		return -1;
	}
	/* obtenemos el nick */
	nick = list[1];
	if (nick.isNull()) {
		debugp ("udbs_RPCaddUser: nick problem\n");
		ASSERT (false);
		return -1;
	}
	/* pres */
	if (resp == NULL || resp->isNull() == false) {
		debugp ("udbs_RPCaddUser: resp problem\n");
		ASSERT (false);
		return -1;
	}
	*resp = PRO_RESP_FALSE;
	/* ahora creamos el usuario que vamos a agregar a la base de datos */
	user = new CUser(&nick, &mac);
	
	if (user == NULL) {
		/* algun error :S */
		return -1;
	}
	/* lo agregamos a la base de datos */
	if (((UDataBase*)db)->addUser (user))
		*resp = PRO_RESP_OK;

	return 0;
	
}


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
	memset (buff, '\0', SSDBP_MAX_BUFF_SIZE);
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
int udbs_RPCWork (SClient * client, UDataBase * udb)
{
	QString resp = "";
	QString req = "";
	QString funcName = "";
	QString * aux = NULL;
	QStringList * list = NULL;
	int result = -2;
	int i = 0;
	bool notFound = true;
	
	
	/* pres */
	if (client == NULL || udb == NULL) {
		ASSERT (false);
		return -2;
	}
	req = client->getData();
	if (udbs_isValidRequest(req) != UDBS_NO_ERROR) {
		ASSERT (false);
		return -1;
	}
	
	/* ahora parseamos el request, y obtenemos la lista */
	aux = udbs_parseRequest (req);
	if (aux == NULL) {
		debugp ("udbs_RPCWork: error udbs_parseRequest returns null\n");
		return -1;
	}
	/* reemplazamos el request, por el ya parseado */
	req = *aux;
	delete aux; aux = NULL;
	
	/* obtenemos la lista */
	list = parseRPCtoList (req);
	if (list == NULL || list->size() < 1) {
		debugp ("udbs_RPCWork: error parseRPCtoList returns null\n");
		return -1;
	}
	/* obtenemos el nombre de la funcion y lo sacamos de la lista */
	funcName = list->front();
	list->removeFirst();
	
	/* ahora buscamos cual es la RPC que tenemos que llamar */
	while (RPC_FUNCS[i].funcName != NULL && notFound) {
		if (funcName == RPC_FUNCS[i].funcName) {
			/* si es igual entonces llamamos a la funcion con los
			 * parametros correspondientes */
			notFound = false;
			result = RPC_FUNCS[i].func (udb, list, &resp);
		}
		i++;
	}
	/* verificamos si encontramos alguna funcion coreespondiente */
	if (notFound) {
		/*! no encontro una funcion :-o, que verdura nos mando?? */
		result = -1;
	}
	/* liberamos un poco de memoria */
	delete list;
	
	/* verificamos que debemos enviar */
	if (result >= 0) {
		/* debemos enviar lo que nos respondieron entonces */
		/*!### Bloqueante ###*/
		result = client->sendData (qstrtochar (resp), resp.size());
	}
	
	return result;
}


