#include "mainprog.h"

/* Constructor, no hace falta nada */
MainProg::MainProg()
{
	this->udb = NULL;
	this->mw = NULL;
	this->btSM = NULL;
	this->running = false;
}

/* Funcion que crea la base de datos.
* REQUIRES:
* 	fname
* 	load (true == load from file, false == new db)
* RETURNS:
* 	< 0	on error
* 	0	if success
*/
int MainProg::createUDB(QString &fname, bool load)
{
	
	if (this->udb != NULL)
		return -1;
		
	
	this->udb = new UDataBase(fname.toStdString().c_str());
	if (this->udb == NULL)
		return -1;
	
	/* verificamos si debemos cargarla o no de disco duro */
	if(load)
		if(!this->udb->loadFromFile(fname.toStdString().c_str())) {
			delete this->udb;
			this->udb = NULL;
			return -1;
		}
	
	/* todo genial */
	return 0;
}

/* Funcion que crea la interfaz grafica 
* RETURNS:
* 	< 0	on error
* 	0	if success
*/
int MainProg::createGUI(QString &confFile)
{
	
	assert(this->udb != NULL);
	
	/* tenemos algo creado? */
	if (this->mw != NULL)
		return -1;
	
	this->mw = new MainWidget(NULL, confFile);
	if (this->mw == NULL)
		return -1;
	
	/* conectamos la señal con el slot de la mw */
	connect((MainProg *) this, SIGNAL(newObjectReceived(DispObject *)),
		 this->mw, SLOT(acceptNewObject(DispObject *)));
	
	
	/* todo genial */
	return 0;
}


/* Funcion que crea el subsistema de recepcion de bluetooth
* y lo inicializa.
* REQUIRES:
* 	load (true => load codes from file, false => newone)
* RETURNS:
* 	-1	error at creation
* 	-2	error at initialization
* 	0	if success
*/
int MainProg::creatBTSubSystem(bool load)
{
	if (this->btSM != NULL)
		return -1;
	
	
	this->btSM = new BTSystemManager(this->udb, load);
	if (this->btSM == NULL)
		return -1;
	
	if (this->btSM->initializeDongles() < 0)
		if (this->btSM->tryToRepairDongles() < 0){
			delete this->btSM;
			this->btSM = NULL;
			return -2;
		}
	
	this->btSM->setMaxConnTime(MAX_CONN_TIME_USCES);
	
	if (this->btSM->startToReceive() < 0) {
		debugp("Error this->btSM->startToReceive ");
		delete this->btSM;
		this->btSM = NULL;
		return -2;
	}
	
	/* todo geniality */
	return 0;
}

/* Funcion que ejecuta en un thread aparte (esta misma clase)
* todo el sistema completo. 
* REQUIRES;
* 	UDB	created
* 	GUI	created
* 	BTSS	created && initialized
* RETURNS:
* 	< 0	on error
* 	0	if success
*/
int MainProg::startSystem(void)
{
	
	assert(this->udb != NULL);
	assert(this->mw != NULL);
	assert(this->btSM != NULL);
	
	/* empezariamos a ejecutar el sistema en un thread aparte */
	this->running = true;
	this->start();
	
	return 0;
	
}
void MainProg::run(void)
{
	DispObject *obj = NULL;
	
	/*! pasos del algoritmo a ejecutar:
	 * 1) obtenemos un DispObject del BTSystemManager
	 * 2) Filtramos el objecto recibido por el DispObjFilter
	 * 3) [Opcional]: guardamos en el StadisticRegister algo.
	 * 4) agregamos el DispObject al MainWidget: (this->mw->addDispObject)
	 * 5) volvemos a 1.
	 */
	
	
	while(this->running) {
		obj = this->btSM->getDispObject();
		if(obj == NULL) {
			debugp("Error al sacar un obj! en el btSM!!!!!!!!!");
			cout << "Tenemos un obj NULL\n";
			continue;
		}
		/* emitimos una señal */
		emit newObjectReceived(obj);
	}
}

/* Funcion que detiene la corrida del sistema (para el thread)
* y ademas deberia fijarse si guardamos los elementos que
* quedaron en la cola del btSM, la lista de codigos usada y
* los usuarios registrados...
* REQUIRES:
* 	startSystem() called
* 	save => guarda las cosas
*/
void MainProg::stopSystem(bool save)
{
	this->running = false;
	/*! FIXME: si esta seteado save => deberiamos guardar todo */
	if (save) {
		this->udb->saveToFile();
	}
}





MainProg::~MainProg()
{
	if(this->udb != NULL)
		delete this->udb;
	if (this->mw != NULL)
		delete this->mw;
	if(this->btSM != NULL)
		delete this->btSM;
	
}

