#include "btobjgenerator.h"


/* Funcion que genera un objeto si lo recibido es un archivo
* REQUIRES:
* 	pkt.cmdType == BT_CMD_FILE
* RETURNS:
* 	NULL	if error
* 	dobj	if success
*/
DispObject *BTObjGenerator::dObjFromFile(BTPaket &pkt)
{
	DispObject *result = NULL;
	
	assert(pkt.getCmdType() == BT_CMD_FILE);
	
	/*! FIXME: !!!!!!!!!!!!!!!!!!!!!!!!! 
	result = new DispObject();
	if (result == NULL)
		return result;
	else {
		QString aux = pkt.getMsg();
		result->setData(aux);
	}
	*/
	return result;
}

/* Funcion que genera un objeto si lo recibido es un texto
* REQUIRES:
* 	pkt.cmdType == BT_CMD_TEXT
* RETURNS:
* 	NULL	if error
* 	dobj	if success
*/
DispObject *BTObjGenerator::dObjFromText(BTPaket &pkt)
{
	DispObject *result = NULL;
	
	assert(pkt.getCmdType() == BT_CMD_TEXT);
	
	result = new DispObject();
	if (result == NULL)
		return result;
	else {
		QString aux = pkt.getMsg();
		result->setData(aux);
	}
	
	return result;
}






/* constructor, necesita de un BTReceiver...
* REQUIRES
* 	receiver != NULL
*/
BTObjGenerator::BTObjGenerator(BTReceiver *receiver, UDataBase *udb);
{
	assert(receiver != NULL);
	assert(udb != NULL);
	
	this->btReceiver = receiver;
	if(this->btReceiver->startListen() < 0)
		debugp("Error al hacer listen en el btReceiver\n");
	this->udb = udb;
}

/*! Funcion bloqueante que lo que hace es simplemente generar
* DispObjects desde las cosas recibidas por medio del 
* BTReceiver..
* REQUIRES:
* 	errCode (< 0 => error; 0 => success
* RETURNS:
* 	NULL	on error
* 	dObj	otherwise
* NOTE: Genera memoria
*/
DispObject *BTObjGenerator::getDispObject(int &errCode)
{
	DispObject *result = NULL;
	bool finish = false;
	char strMac[20] = {0};
	bdaddr_t mac;
	BTPaket pkt;
	QString qMac = "";
	CUser *user = NULL;
	
	while (!finish) {
		errCode = this->btReceiver->getReceivedObject(pkt, &mac);
		if (errCode < 0) {
			/* error */
			return NULL;
		}
		/* vemos que tipo de comando estamos recibiendo; */
		switch(pkt.getCmdType()) {
			case BT_CMD_TEXT:
				result = dObjFromText(pkt);
				if (result != NULL)
					result->kind = DISPOBJ_TEXT;
				break;
				
			case BT_CMD_FILE:
				result = dObjFromFile(pkt);
				if (result != NULL)
					result->kind = DISPOBJ_PICTURE;
				break;
				
			default:
				/*! por el momento nada... */
				break;
		}
		if(result == NULL) {
			errCode = -4;
			return NULL;
		}
		/* si estamos aca entonces es porque debemos agregar si o si
		 * el user al DispObject */
		ba2str(&mac, strMac);
		qMac = strMac;
		user = this->udb->getUserFromMac(qMac);
		if(user == NULL) {
			errCode = -3;
			delete result;
			return result;
		}
		result->setUser(user);
		
		finish = true;
	}
	
	return result;
}


/* destructor */
BTObjGenerator::~BTObjGenerator(void)
{
	return;
}