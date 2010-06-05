#include "btreceiver.h"

/*!	###		FUNCIONES PRIVADAS		###	*/

/* Funcion que se encarga de inicializar un dongle device 
* REQUIRES:
* 	dongle 	!= NULL
* RETURNS:
* 	< 0	on error
*	0	if success
*/
int BTReceiver::initializeDongle(BTDongleDevice *dongle)
{
	int result = -1;
	
	assert(dongle != NULL);
	
	/* lo hacemos visible */
	result = dongle->makeDevDiscoverable();
	if (result < 0) {
		string *devName = dongle->getDevName();
		if (devName) {
			fprintf(stderr,"%s:error al hacer visible el dongle\n",
				devName->c_str());
			delete devName;
		}
		return result;
	}
	return 0;
}

/* Funcion que setea una SDP asociada a un server. 
* REQUIRES:
* 	UUID	!= NULL
* 	dongle 	!= NULL
* 	port
* RETURNS:
* 	0	if success
* 	< 0	on error
*/
int BTReceiver::createSdpSession(uint32_t *uuid, BTDongleDevice *dongle, 
		      int port)
{
	BTSDPSessionData *sdpS = NULL;
	
	
	assert(uuid != NULL);
	assert(dongle != NULL);
	
	sdpS = new BTSDPSessionData(uuid, (uint8_t) port);
	if (!sdpS) {
		debugp("no se pudo crear la SDP Session\n");
		return -1;
	}
	/* se pudo crear correctamente ==> creamos el server */
	
	/*! deberiamos mostrar ahora el servicio */
	if (dongle->addSDPSession(sdpS) < 0){
		debugp("Error al intentar publicar el servicio\n");
		delete sdpS;
		return -2;
	}
	
	return 0;
}

/* Funcion que genera e inicializa un server para un dongle
* determinado.
* REQIURES:
* 	dongle 	!= NULL
* RETURNS:
* 	server 	!= NULL	on success
* 	NULL		on errorr
*/
BTSimpleServer *BTReceiver::createServer(BTDongleDevice *dongle)
{
	BTSimpleServer *server = NULL;
	uint32_t sdpUUID[4] = {BTRECEIVER_UUID};
	
	assert(dongle != NULL);
	
	/* verificamos que este funcionando bien el dongle */
	if (dongle->getMac() == NULL)
		return NULL;
	
	server = new BTSimpleServer(dongle->getMac(), 8, BTRECEIVER_SERVER_PORT);
	
	if(server == NULL)
		return server;
	
	/* ahora vamos hacer el bind y el listen */
	if(server->bindSocket() < 0) {
		debugp("Error al hacer el bind en el server\n");
		delete server;
		return NULL;
	}
	if (server->startListen() < 0) {
		debugp("Error al hacer el listen en el server\n");
		delete server;
		return NULL;
	}
	if (createSdpSession(sdpUUID, dongle, BTRECEIVER_SERVER_PORT) 
		< 0){
		debugp("Error al crear la SDPSessionData\n");
		delete server;
		return NULL;
	}
	
	return server;
}

/* Funcion que se encarga de determinar si una mac es admisible
* o no (se encuentra en la base de datos).
* RETURNS:
* 	true	if is in the db
* 	false	otherwise
*/
bool BTReceiver::isMacInDB(const bdaddr_t &mac)
{
	char strMac[20] = {0};
	CUser * user = NULL;
	bool result = false;
	
	ba2str(&mac, strMac);
	user = new CUser("", strMac);
	if(user != NULL) {
		result = this->udb->existUser(user);
		delete user;
	}
	
	return result;
}


/* Funcion que envia un string con todos los dongles del sistema
* formateados y respetando el protocolo listo para mandar
* a una conexion.
* REQUIRES:
* 	con 	!= NULL
*/
void BTReceiver::sendDongleList(BTConnection *con)
{
	
	assert(con != NULL);
	con->sendData(this->donglesMacs);
	this->connManager.setFlags(con, BTCM_POLL_OUT_FLAGS);
}

/* Funcion que registra un nuevo usuario en la base de datos
* REQUIRES:
* 	pkt
* RETURNS:
* 	< 0	on error
* 	0	if succes
*/
int BTReceiver::registerNewUser(const bdaddr_t *mac, BTPaket &pkt)
{
	CUser *user = NULL;
	string aux = "";
	int p = 0;
	char strMac[20] = {0};
	
	if (pkt.getCmdType() != BT_CMD_REGI)
		return -1;
	
	/* separamos el codigo y el nick */
	p = pkt.getMsg().find(" ");
	if (p <= 0)
		return -1;
	
	/*! verificamos si el codigo que enviaron es valido o si fue usado */
	aux = pkt.getMsg().substr(0, p);
	if (this->codAdmin.isCodeValid(aux) == false || 
		this->codAdmin.wasUsed(aux)) {
		/* el codigo no es valido o ya fue usado! */
		return -1;
	}
	/*! si estamos aca es valido => lo agregamos a la lista */
	this->codAdmin.addUsedCode(aux);
	/* extraemos el nick y creamos el user */
	p++;
	aux = pkt.getMsg().substr(p, pkt.getMsg().size() - p);
	ba2str(mac, strMac);
	user = new CUser(aux.c_str(), strMac);
	if (user == NULL)
		return -1;
	
	/* agregamos el usuario */
	this->udb->addUser(user);
	
	return 0;
}


/* Funcion que dada una conexion, determina si la conexion debe
* ser cerrada, debe seguir recibiendo datos, o si ya tiene
* datos completos y rellena el paket, ademas de que elimina
* del buffer de recepcion los datos.
* REQUIRES:
* 	con != NULL
* RETURNS:
* 	-1	si la conexion debe ser cerrada
* 	0	si tenemos datos completos.
* 	1	si tenemos datos impletos pero correctos.
* 	pkt 	paquete extraido (si no hubo error).
*/
int BTReceiver::checkConnection(BTConnection *con, BTPaket &pkt)
{
	int result = 0;
	int cmd = -1;
	int size = 0;
	
	
	assert(con != NULL);
	
	/* primero que todo vamos a verificar si estamos recibiendo bien. */
	result = btproto_parse_data(con->getRcvBuff(), cmd, size, pkt.getMsg());
	pkt.setCmdType(cmd);
	pkt.setOrigination(BT_PKT_RCV);
	if (result < 0 || cmd < 0) 
		/* si hay que cerrar la conexion */
		return result;
	
	
	/*! deberiamos ahora analizar los 3 posibles casos que tenemos.
	 * 1) Tenemos un pedido de la lista de donglesMacs.
	 * 2) Se estan intentando registrar
	 * 3) Estan mandando alguna otra cosa.
	 */
	switch(cmd) {
		case BT_CMD_REQU:
			if(pkt.getMsg().compare("get_server_list") == 0){
				/* enviamos la lista de servers y devolvemos
				 */
				sendDongleList(con);
				/*! FIXME:debemos esperar que se envien para 
				 * cerrar la conexion, */
			}
			/* en cualquier caso cerramos la conexion */
			result = 1;
			break;
		case BT_CMD_REGI:
			/* se estan registrando... */
			result = registerNewUser(con->getMacDest(), pkt);
			result = -1;
			break;
			
		default:
			/* estan mandando otra cosa... verificamos que este 
			 * en la base de datos para poder hacerlo */
			if (!isMacInDB(*(con->getMacDest())))
				result = -1;
			else
				result = 0;
			break;
	}
	
	return result;
}



/*!	###		FUNCIONES PUBLICAS		###	*/


/* constructor */
BTReceiver::BTReceiver(UDataBase *udb)
{
	list<BTDongleDevice *>::iterator it;
	list<BTDongleDevice *> *dongleList = NULL;
	char addr[20] = {0};
	string *aux = NULL;
	
	this->dManager = new BTDManager();
	assert(udb != NULL);
	this->udb = udb;
	this->donglesMacs.clear();
		
	/* inicializamos los dongles */
	dongleList = this->dManager->getListDevices(0);
	
	if(dongleList == NULL) {
		debugp("Error al obtener la lista de dongles\n");
		return;
	}
	/* ahora para cada dongle creamos un server */
	for (it = dongleList->begin(); it != dongleList->end(); ++it){
		initializeDongle(*it);
		/* extraemos su mac */
		if (ba2str((*it)->getMac(), addr) < 0){
			debugp("No se pudo sacar la mac?\n");
			continue;
		}
		this->donglesMacs.append(addr);
		this->donglesMacs.append(",");
	}
	delete dongleList;
	/* ahora parseamos para que este lista para ser mandada la lista */
	aux = btproto_create_data(this->donglesMacs, BT_CMD_RESP);
	assert(aux != NULL);
	this->donglesMacs = *aux;
	delete aux;
}

/* Funcion que genera los servidores, configura los dongles, y 
* comienza a escuchar. Seria la funcion de inicializacion.
* RETURNS:
*	< 0	on error
*	0	otherwise
*/
int BTReceiver::startListen(void)
{
	list<BTDongleDevice *> *dongleList = NULL;
	list<BTDongleDevice *>::iterator it;
	BTSimpleServer *server = NULL;
	
	
 	stopListen();
	/*! FIXME: aca solo deberiamos obtener los dongles que esten "habilitados
	 *	o en caso de que no lo esten entonces habilitarlos.
	 */
	dongleList = this->dManager->getListDevices(0);
	
	if(dongleList == NULL) {
		debugp("Error al obtener la lista de dongles\n");
		return -1;
	}
	/* ahora para cada dongle creamos un server */
	for (it = dongleList->begin(); it != dongleList->end(); ++it){
		server = createServer((*it));
		if (server == NULL) {
			/* error al intentar hacer un server aca */
			debugp("Error al crear el server\n");
			continue;
		}
		/* agregamos al manejador de conexiones */
		this->connManager.insertServer(server);
	}
	delete dongleList;
	
	return 0;
}

/* Funcion que detiene todos los servidores.
* RETURNS:
*	0	if success
*/
int BTReceiver::stopListen(void)
{
	list<BTSimpleServer *> sList = this->connManager.getServersList();
	list<BTConnection *> cList = this->connManager.getConList();
	list<BTSimpleServer *>::iterator it;
	list<BTConnection *>::iterator cit;
	
	
	for (it = sList.begin(); it != sList.end(); ++it) {
			assert((*it) != NULL);
			/* eliminamos el server */
			this->connManager.removeServer(*it);
			/* lo borramos */
			delete (*it);
	}

	for (cit = cList.begin(); cit != cList.end(); ++cit) {
			assert((*cit) != NULL);
			/* eliminamos el server */
			this->connManager.removeConnection(*cit);
			delete (*cit);
	}
	

	return 0;
}

/* Funcion que devuelve ante una nuva recepcion de datos, los
* datos recibidos por la conexion y la direccion de la misma.
* NOTE: Las nuevas conexiones/envio de datos se manejan 
* 	 automaticamente.
* REQUIRES:
* 	mac	!= NULL
* RETURNS:
* 	< 0 	on error
* 	0	if success
* 	pkt 	= paket received
* 	mac	= remote mac address
*/ 
int BTReceiver::getReceivedObject(BTPaket &pkt, bdaddr_t *mac)
{
	bool finish = false, fail = false;
	BTSimpleServer *server = NULL;
	BTConnection *con = NULL;
	int result = 0;
	eventType_t event = BTCM_EV_ERR;
	int tries = 0;
	
	assert(mac != NULL);
	
	while(!finish && !fail) {
		con = this->connManager.getConnEvent(event, result);
		if (con != NULL)
			bacpy(mac, con->getMacDest());
		switch(event) {
			case BTCM_EV_NEW_CONN:
				/*! tenemos una nueva conexion, no tenemos que
				 * hacer nada...*/
				continue;
				break;
				
			case BTCM_EV_OUT:
				/*! se produjo un evento de salida, probablemente
				 * es porque nos cerraron la conexion?...
				 */
				if (result < 0) {
					/* nos cerraron la conexion */
					this->connManager.removeConnection(con);
					delete con;
				}
				break;
				
			case BTCM_EV_CLOSE_CONN:
				/*! una conexion fue cerrada, debemos ver si
				 * se cerro tan rapido que no alcanzo a producirse
				 * un evento de EV_RCV, asique analizamos los
				 * datos que tiene en el buffer.
				 */
				if ((int)con->getRcvBuff().size() > 0) {
					/* tenemos datos que analizar => */
					int status = checkConnection(con, pkt);
					if (status == 0) {
						/* entonces tenemos algo que
						 * sirve y es lo que tenemos
						 * que devolver */
						finish = true;
					} else if (status == 1) {
						/* debemos seguir recibiendo,
						 * pero no se puede... */
					} 
				}
				
				/* no hay que hacer nada, simplemente eliminamos
				 * la conexion.. */
				this->connManager.removeConnection(con);
				delete con;
				break;
				
			case BTCM_EV_RCV:
				/*! recibimos datos, debemos verificar si estan
				 * completos, o debemos seguir recibiendo o
				 * si debemos cerrar la conexion */
				if ((int)con->getRcvBuff().size() > 0) {
					/* tenemos datos que analizar => */
					int status = checkConnection(con, pkt);
					if (status == 0) {
						/* entonces tenemos algo que
						* sirve y es lo que tenemos
						* que devolver si y solo si no
						* nos estan pidiendo la lista
						* de servers. */
						/*! FIXME: eliminamos todo el 
						 * buffer, deberiamos eliminar 
						 * solo el buffer recibido */
						con->clearRecvBuffer();
						
						finish = true;
					} else if (status == 1) {
						/* debemos seguir recibiendo,
						 * asique no hacemos nada..*/
					} else if (status == -1) {
						/* tenemos que cerrarla al chori */
						this->connManager.removeConnection(con);
						delete con;
					}
				} else {
					/* debemos eliminar la conn */
					this->connManager.removeConnection(con);
					delete con;
				}
				break;
				
			case BTCM_EV_SERVER_ERR:
			{
				BTDongleDevice *dongle = NULL;
				BTSimpleServer *newServer = NULL;
				
				tries++;
				if(tries >= BTRECEIVER_MAX_TRIES) {
					finish = true;
					fail = true;
				}
				/*! vamos a eliminar el server que dio error */
				server = this->connManager.getServerFromFd(result);
				/* ahora inicializamos el server nuevamente */
				assert(server != NULL);
				this->connManager.removeServer(server);
				dongle = this->dManager->getDongleFromMac(
					server->getDongleMac());
				delete server;
				newServer = createServer(dongle);
				if(newServer == NULL){
					fail = true;
					finish = true;
					break;
				}
				this->connManager.insertServer(newServer);
				if(tries >= BTRECEIVER_MAX_TRIES) {
					finish = true;
					fail = true;
				}
				
				/* deberiamos generar un nuevo server para el
				 * dongle que dejo de existir... */
				break;
			}
			case BTCM_EV_ACCEPT_ERR:
				tries++;
				if (tries >= BTRECEIVER_MAX_TRIES){
					finish = true;
					fail = true;
				}
				break;
				
			case BTCM_EV_ERR:
				debugp("Error interno BTReceiver\n");
				finish = true;
				fail = true;
				break;
			default:
				assert(false);
				break;
		}
		
	}
	if (fail)
		/* no se produjo ningun error-.. */
		result = -1;
	else
		result = 0;
	
	return result;
}


/* Funcion que devuelve la lista de conexiones con la que esta
* trabajando:
* NOTE: TENER CUIDADO con manipular conexiones cuando se esta
* 	 llamando a getReceivedObject desde otro thread.
* NOTE 2: NO borrar ninguna conexion.
*/
const list<BTConnection *>& BTReceiver::getConList(void)
{
	return this->connManager.getConList();
}

/* destructor */
BTReceiver::~BTReceiver(void)
{
	stopListen();

	if (this->dManager != NULL)
		delete this->dManager;
	
}



