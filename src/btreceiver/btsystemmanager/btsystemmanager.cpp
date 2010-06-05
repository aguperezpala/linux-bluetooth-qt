#include "btsystemmanager.h"


/* funcion que setea un string como error y cambia el estado
* a algun estado determinado.
* REQUIRES:
* 	msg 	!= NULL
*/
void BTSystemManager::setMsgAndState(const char *msg, int state)
{
	assert(msg != NULL);
	
	this->report = msg;
	this->status = state;
}

/* Funcion que va a cerrar toda aquella conexion que exceda 
* maxConnTime.
* REQUIRES:
* 	list<BTConnection *>
* RETURNS:
* 	n > 0	number of connection closed
* 	< 0	on error
*/
int BTSystemManager::closeConnTimeExceeded(const list<BTConnection *> &l)
{
	int result = 0;
	list<BTConnection *>::const_iterator it;
	
	if(l.size() == 0)
		return 0;
	
	for(it = l.begin(); it != l.end(); ++it) {
		if((*it) == NULL)
			continue;
		/* verificamos si la conexion se excedio del tiempo de conexion */
		if (((*it)->getLastRecvTime() > this->maxConnTime) &&
			((*it)->getLastSendTime() > this->maxConnTime)) {
			/* hay que cerrarla */
			(*it)->closeConnection();
			result++;
		}
	}
	return result;
}




/* constructor:
* REQUIRES:
* 	udb != NULL
*/
BTSystemManager::BTSystemManager(UDataBase *udb)
{
	assert(udb != NULL);
	this->udb = udb;
	
	this->btRec = new BTReceiver(udb);
	if(this->btRec == NULL) {
		setMsgAndState("Error al crear el BTReceiver", BTSM_ERROR);
		return;
	}
	
	this->btObjGen = new BTObjGenerator(this->btRec, this->udb);
	if(this->btObjGen  == NULL) {
		setMsgAndState("Error al crear el BTObjGenerator", BTSM_ERROR);
		return;
	}
	
	this->report = "";
	this->status = BTSM_STOP;
	this->dobjsList.clear();
	
	/*! debemos inicializar el mutex bloqueado para que no puedan extraer
	 * nada al principio */
	this->auxMutex.lock();
	
}

/* Funcion que devuelve un reporte de la ultima accion
* realizada. */
QString &BTSystemManager::getReport(void)
{
	return this->report;
}

/* Funcion que verifica los dongles del sistema y los inicializa
* RETURNS:
* 	< 0	si se encontro algun fallo
* 	0	si no hubo fallos.
* NOTE: deja en report el reporte obtenido.
*/
int BTSystemManager::initializeDongles(void)
{
	int result = 0, dErrs = 0;
	BTDManager *dm = NULL;
	list<BTDongleDevice *> *dongleList = NULL;
	list<BTDongleDevice *>::iterator it;
	QString info = "";
	
	if (this->btRec == NULL) {
		setMsgAndState("No existe el BTReceiver...", BTSM_ERROR);
		return -1;
	}
	dm = this->btRec->getDongleManager();
	if (dm == NULL) {
		setMsgAndState("No existe el BTDongleManager...", BTSM_ERROR);
		return -1;
	}
	
	dongleList = dm->getListDevices(0);
	if(dongleList == NULL){
		setMsgAndState("No hay dongles en el sistema", BTSM_ERROR);
		return -1;
	}
	for(it = dongleList->begin(); it != dongleList->end(); ++it) {
		result = 0;
		if ((*it)->getStatus() != 0) {
			info += "Dongle ID: ";
			info += (int) (*it)->getID();
			info += " no se pudo obtener el estado\n";
			dErrs++;
			continue;
		}
		result = (*it)->makeDevDiscoverable();
		if (result < 0) {
			string *devName = (*it)->getDevName();
			if (devName) {
				info += "Dongle: ";
				info += devName->c_str();
				info += "\t no se pudo hacer visible\n";
				delete devName;
			}
		}
		/*! que mas se hace aca? */
	}
	this->report = info;
	delete dongleList;
	
	if (dErrs >= dm->getNumberOfDevices(-1)) {
		this->report += "\nHubo tantos errores como dongles!\n";
		this->status = BTSM_ERROR;
		return -1;
	}
	
	this->status = BTSM_INITIALIZED;
	return 0;
}

/* Funcion que intenta reparar los dongles del sistema 
* dandole de baja o haciendo lo necesario para intentar hacer
* que funcione.
* RETURNS:
* 	< 0	on error
* 	0	if succes
* NOTE: deja en report los resultados
* FIXME: hacer esta funcion
*/
int BTSystemManager::tryToRepairDongles(void)
{
	return -1;
}

/*! Funcion que Ejecuta el BTReceiver y comienza a recibir.
* Ademas chequea las conexiones (toda aquella conexion que
* este mas de n msecs sera desconectada automaticamente)
* NOTE: Corre en un thread
* REQUIRES:
* 	status != (ERROR | RUNNING)
* RETURNS:
* 	< 0	on error
* 	0	if success
*/
int BTSystemManager::startToReceive(void)
{
	assert(this->status != BTSM_ERROR);
	assert(this->status != BTSM_RUNNING);
	assert(this->btRec != NULL);
	
	if (this->btRec->startListen() < 0){
		setMsgAndState("Error al inicializar el BTReceiver",BTSM_ERROR);
		return -1;
	}
	
	this->canReceive = true;
	/* ejecutamos el thread */
	this->start();
}
void BTSystemManager::run(void)
{
	DispObject *obj = NULL;
	int errCode = 0;
	int tries = 5;
	
	
	if(this->btObjGen == NULL) {
		setMsgAndState("Error al crear el BTObjGenerator", BTSM_ERROR);
		return;
	}
	/*! aca es donde se ejecuta el algoritmo principal, notar que la forma
	 * en que vamos a determinar que conexiones cerrar es medio kako!, ya que
	 * solo vamos a chequear las conexiones ante determinados eventos y 
	 * lo podemos hacer porque no hay multithreading!.
	 * Una vez que se cierra la conexion, en la proxima ejecucion del
	 * BTReceiver le deberia saltar un error y manejarlo correctamente.
	 * FIXME (arreglar eso del chequeo de las conexiones) */
	this->dispMutex.unlock();
	while (this->canReceive) {
		/* obtenemos el proximo objeto */
		obj = NULL;
		obj = this->btObjGen->getDispObject(errCode);
		
		if (obj == NULL){
			tries--;
			if(tries <= 0) {
				setMsgAndState("Error al al recibir objetos desde"
				" el BTObjGenerator", BTSM_ERROR);
				return;
			}
			continue;
		}
		
		tries = 5;
		
		/* bloqueamos la lista para agregar un objeto */
		this->dispMutex.lock();
		this->dobjsList.append(obj);
		this->dispMutex.unlock();
		
		/* liberamos el mutex por si alguien intento extraer algo y no
		 * habia */
		this->auxMutex.unlock();
		
		/* revisamos ahora las conexiones para cerrar las que 
		 * hayan excedido el tiempo limite */
		closeConnTimeExceeded(this->btRec->getConList());
	}
	
	this->btRec->stopListen();
	

}


/* Funcion que detiene el BTReceiver y termina el thread. 
*/
void BTSystemManager::stopReceiving(void)
{
	this->canReceive = false;
	/* deberiamos terminar de recibir asi: */
	/* terminar el thread */
	this->status = BTSM_STOP;
}


/*! Funcion que devuelve un elemento de la lista si es que hay,
* caso contrario se bloquea la llamada hasta que exista un
* elemento.
* RETURNS:
* 	dObj
*/
DispObject *BTSystemManager::getDispObject(void)
{
	DispObject *result = NULL;
	
	if(this->dobjsList.empty())
		this->auxMutex.lock();
	
	this->dispMutex.lock();
	result = this->dobjsList.takeFirst();
	this->dispMutex.unlock();
	
	return result;
}

/* Funcion que devuelve la cantidad de de DispObjects 
* disponibles */
int BTSystemManager::getNumDispObjs(void)
{
	return this->dobjsList.size();
}

/* destructor
* NOTE: libera el btReceiver y el BtObjGen 
* */
BTSystemManager::~BTSystemManager(void)
{
	QList<DispObject *>::iterator it;
	
	
	this->terminate();
	this->wait();
	if (this->btRec != NULL)
		delete this->btRec;
	
	if (this->btObjGen != NULL)
		delete this->btObjGen;
	
	for(it = this->dobjsList.begin(); it != this->dobjsList.end(); ++it)
		if (*it != NULL)
			delete *it;
}