#include "obexreceiver.h"


/* Funcion que genera un DispObject desde una QStringList,
* chequeando que respete algunas de las condiciones que
* pedimos.
* REQUIRES:
*	strList != NULL
* RETURNS:
*	NULL	on error (no tenemos los datos que requerimos
*			  osea que no respeta el protocolo)
*	dobj	on success (NOTE: Alloca memoria)
*/
DispObject * ObexReceiver::dispObjFromStrList (QStringList * strList)
{
	DispObject * result = NULL;
	QString fname = "";
	QString MAC = "";
	CUser * user = NULL;
	
	/* PRE */
	if (strList == NULL) {
		ASSERT (false);
		return result;
	}
	
	/*! ACA CAMBIAR ESTA PARTE SI QUEREMOS RESPETAR OTRO PROTOCOLO */
	/* primero nos vamos a asegurar que tenga tamaño 2 */
	if (strList->size() != 2) {
		debugp ("ObexReceiver::dispObjFromStrList: listSize err\n");
		return result;
	}
	/* si estamos aca entonces es porque tenemos algo posiblemente bien...
	 * obtenemos el nombre del archivo y la MAC
	 */
	fname = strList->front();
	MAC = strList->last();
	/* verificamos que sea correcto... */
	/*! deberia existir el archivo... */
	if (!QFile::exists (fname)) {
		/* el archivo no existe :( */
		debugp ("ObexReceiver:dispObjFromStrList: no existe archivo\n");
		return result;
	}
	/* verificamos el tamaño de la MAC */
	if (MAC.size() != MAC_SIZE) {
		debugp ("ObexReceiver:dispObjFromStrList: MAC.size() "
			"incorrec\n");
		return result;
	}
	/*! pareceria andar, buscamos el usuario ahora en la udb */
	user = this->db->getUserFromMac (MAC);
	if (user == NULL) {
		/*!### ZATANAS, algo esta como el remil ocotaso, estamos
		 * recibiendo un usuario que en teoria el obexserver lo ve
		 * pero nosotros ni chori... :(
		 */
		debugp ("ObexReceiver:dispObjFromStrList: User == NULL\n");
		return result;
	}
	
	/*! listo, tenemos todo perfecto => devolvemos lo que queremos */
	result = new DispObject (fname);
	result->setUser (user);
	
	return result;
}




/* Constructor
* Inicializa las variables del servidor
*/
ObexReceiver::ObexReceiver(UDataBase * udb)
{
	if (udb == NULL) {
		debugp ("ObexReceiver::ObexReceiver: udb NULL!\n");
		return;
	}
	this->db = udb;
	this->server = new SServer(OBREC_BUFF_SIZE);
	ASSERT (this->server != NULL);
	this->client = NULL;
	this->buff.clear();
}
	


/* Funcion que sirve para reiniciar el server, en caso de que
* este abierto, esta funcion lo cierra y comienza de nuevo
* a escuchar.
* Es una funcion de fuerza "bruta"... no se recomienda :S
*/
void ObexReceiver::resetServer (void)
{
	/* vamos a comenzar todo de nuevo */
	if (this->server) {
		this->server->stopServer();
		delete this->server;
		this->server = NULL;
	}
	if (this->client) {
		delete this->client;
		this->client = NULL;
	}
	this->buff.clear();
	
	/* reiniciarmos */
	startReceiver();
	
}


/* Funcion que inicializa el server y se bloquea hasta que		
* algun cliente se conecta.
* RETURNS;
*	true 	on success
*	false	on error
* NOTE: ES BLOQUEANTE
*/
bool ObexReceiver::startReceiver(void)
{
	bool result = false;
	unsigned short port = 0;
	bool listening = false;;
	
	if (!this->server) {
		debugp ("ObexReceiver::startReceiver: server NULL\n");
		return false;
	}
	/* frenamos el server por si esta corriendo */
	this->server->stopServer();
	
	for (port = OBREC_START_PORT; port < OBREC_END_PORT && !listening
		; port ++) {
		listening = this->server->startListen(port);
	}
	
	if (!listening)
		/* si no pudimos escuchar en ningun puerto volvemos */
		return result;
	
	this->client = this->server->acceptClient();
	result = (this->client != NULL);
	
	return result;
}
	
	

/* Funcion que devuelve un DispObject *. Lo que hace es leer
* del buffer si tenemos suficiente info como para armar un
* DispObject. Si es asi devolvemos automaticamente y eliminamos
* del buffer lo obtenido.
* Si no tenemos suficiente info => leemos del socket eso lo
* hace ### BLOQUEANTE ###
* Ademas suprime todo tipo de error, sigue escuchando.
* RETURNS:
* 	dobj 	!= NULL (si todo andubo bien, casi siempre)
* 	NULL 	si hubo error interno (poco probable).
*//*! Esta es la funcion principal, hace todo practicamente */
DispObject * ObexReceiver::getDispObj (void)
{
	DispObject * result = NULL;
	QStringList * list = NULL;
	int errCount = 8;	/* cantidad de errores que vamos a tolerar */
	bool havePkt = false;
	int bytesReaded = 0;
	bool error = false;
	
	
	/* Lo que vamos a hacer es:
	 * En caso de erro vamos a aceptar hasta errCount errores posibles, lo 
	 * que vamos hacer despues es hacer un accept (en caso de error) si no
	 * recibimos datos, hasta que el  parser nos encuentra algo. 
	 * A medida que vayamos leyendo vamos "encolando" en nuestro buffer 
	 * interno para luego ir parseandolo y extrayendo los paquetes.
	 */
	 
	while (!havePkt && errCount > 0) {
		/* inicializamos esta variable */
		error = false;
		
		if (!this->client) {
			errCount--;
			/* reintentamos */
			this->client = this->server->acceptClient();
			/* reiniciamos el buffer */
			this->buff.clear();
			/* pudimos ? */
			if (!this->client)
				continue;
		}
		/* si pudimos... veamos de leer datos */
		bytesReaded = this->client->readData();
		if (bytesReaded == 0) {
			/* nos cerraron la conexion => */
			delete this->client; this->client = NULL;
			/*! no lo consideramos un error... */
			errCount++;
			continue;
		} else if (bytesReaded < 0) {
			/* si es un error */
			delete this->client; this->client = NULL;
			continue;
		}
		/* si estamos aca es porque leimos :) */
		this->buff.append (QByteArray::QByteArray(
						this->client->getData(),
						this->client->getDataSize()));
		/* aca en teoria tenemos el ultimo buffer, vamos a ver si
		 * tenemos un DispObject */
		list = obpa_parse (this->buff, error);
		
		if (error) {
			/* mmm tenemos un error de protocolo o algo raro =>
			 * mandamos al chori al cliente */
			delete this->client; this->client = NULL;
			continue;
		}
		
		/* ahora hay una posibilidad, que no haya error pero que no
		 * tengamos los datos completos => verificamos esto */
		if (list == NULL)
			continue;
		/*! no es NULL => TENEMOS una supuesta lista, verificamos ahora
		 * que la lista "corresponda" a lo que esperabamos.
		 */
		result = dispObjFromStrList (list);
		/* liberamos la lista */
		delete list; list = NULL;
		/* verificamos que tengamos un pkt, de lo contrario es porque
		 * no estan respetando el protocolo.... */
		if (result == NULL)
			/* no respeta el protocolo */
			errCount--;
			/*! NOTE: aca tenemos que determinar la politica de
			si cerramos o no la conexion, si la cerramos es zatanas
			si lo perdonamos, vamos a perdonarlo errCount veces */
		else
			/* tenemos un pakete :) */
			havePkt = true;
	}
	
	/* hacemos unos ultimos chequeos */
	if (!havePkt && this->client) {
		debugp ("ObexReceiver::getDispObj: El cliente cometio muchos "
		"errores, lo matamos....\n");
		/* se arto de cometer errores => lo matamos */
		delete this->client; this->client = NULL;
	}
	
	return result;
}


/* Destructor.
* Elimina el cliente, los datos, y el servidor, cierra tambien
* la conexion.
*/
ObexReceiver::~ObexReceiver()
{
	if (this->server)
		delete this->server;
	if (this->client)
		delete this->client;
	
}



