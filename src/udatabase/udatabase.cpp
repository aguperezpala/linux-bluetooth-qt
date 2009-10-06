#include "udatabase.h"



/* Funcion que intenta agregar un usuario a la user database
* fijandose que sean validas la MAC y NICK
* RETURNS:
* 	true 	if NO error
*	false	if error
*/
bool UDataBase::udb_add_user (QString * MAC, QString * nick)
{
	CUser * user = NULL;
	
	if (MAC == NULL || nick == NULL)
		return false;	/* error */
	
	if (MAC->length() != MAC_ADDR_SIZE)
		return false;	/* error */
			
	/* creamos el usuario */
	user = new CUser (nick, MAC);
	if (user == NULL)
		return false;
	/* intentamos agregar a la hash */
	this->addUser (user);
	
	/* limpiamos un poco */
	MAC->clear();
	nick->clear();
	
	return true;
}



/* Constructor: Se va a pedir un archivo de texto, el cual
* a medida que se vayan ingresando los usuarios se van a ir
* guardando en el mismo
* REQUIRES:
*	fname != NULL
*/
UDataBase::UDataBase (const char * fname)
{
	this->file = NULL;
	this->serverOn = false;
	/* pre */
	ASSERT(fname != NULL);
	
	this->fname = QString (fname);
	/* abrimos el archivo para leer y escribir al final del fichero */
	this->file = fopen (fname, "a+");
	if (this->file == NULL) {
		debugp ("UDataBase: Error al intentar abrir el archivo\n");
		ASSERT (false);
	}
}

/* Funcion que chequea si existe un usuario o no.
* REQUIRES:
* 	user != NULL
* RETURNS:
*	true 	if exist
*	false 	otherwise
*/
bool UDataBase::existUser (const CUser * user)
{
	bool result = false;
	
	/* pre */
	ASSERT (user != NULL);
	
	/*! atomico */
	this->mutex.lock();
	result = this->hashTable.contains (user->getMAC());
	this->mutex.unlock();
	
	return result;
}

/* Funcion que va a agregar a un usuario SI Y SOLO SI no se 
* encuentra en la base de datos, Si existe no lo agrega.
* REQUIRES:
* 	user != NULL
* RETURNS:
*	true 	si el usuario fue agregado
*	false	caso contrario (ej ya existia o error)
* NOTE: NO se debe liberar user despues de haber llamado a esta
* funcion.
*/
bool UDataBase::addUser (CUser * user)
{
	bool result = false;
	
	/* pres */
	ASSERT (user != NULL);
	
	/* verificamos si existe o no en la hashTable */
	if (existUser (user))
		/* lo borramos al choripan */
		delete user;
	else {
		/*! hacemos atomico esto */
		this->mutex.lock();
		/* agregamos a la hash una nueva entrada */
		this->hashTable.insert (user->getMAC(), user);
		this->mutex.unlock();
		
		result = true;
		
	}
	
	return result;
}

/* Funcion que devuelve un usuario si existe en la base de datos
* a partir de la MAC.
* REQUIRES:
*	MAC.isNull() == false
* RETURNS:
*	usr->copy()	si existe el usuario.
*	NULL		si no existe.
* NOTE: devuelve una copia => GENERA MEMORIA
*/
CUser * UDataBase::getUserFromMac (QString & MAC)
{
	CUser * usr = NULL;
	bool exist = false;
	
	/* pre */
	ASSERT (MAC.isNull() == false);
	
	/* nos fijamos si existe, si no devolvemos null de pecho */
	if (MAC.isNull())
		return usr;
	
	this->mutex.lock();
	exist = this->hashTable.contains (MAC);
	this->mutex.unlock();
	
	if (!exist)
		return usr;
	
	/* si estamos aca es porque si existe => lo obtenemos */
	this->mutex.lock();
	usr = this->hashTable.value (MAC);
	this->mutex.unlock();

	/* devolvemos una copia */
	return usr->copy();
}


/* Funcion que carga una base de datos desde un archivo
* NOTE: Agrega a la lista ya existente, no BORRA lo que hay
* en la lista actual.
* REQUIRES:
*	fname != NULL
* RETURNS:
*	true 	if success
*	false 	otherwise
*/
bool UDataBase::loadFromFile (const char * fname)
{
	FILE * fd = NULL;
	QString MAC, nick;
	char buff[300]; /* no deberia tener mas que esto cada linea */
	int c = 0, p = 0;
	bool success = true;
	
	
	/* pre */
	ASSERT (fname != NULL);
	
	memset (buff, '\0', 300);
	
	/* abrimos el archivo */
	fd = fopen (fname, "r");
	if (fd == NULL) {
		/* error */
		debugp ("UDataBase::loadFromFile: error al abrir el archivo\n");
		return false;
	}
	
	/* ahora leemos linea por linea y vamos generando usuarios y 
	 * metiendolos a la lista si es que existen */
	c = fgetc(fd);
	while ((c != EOF) && success) {
		buff[p] = (char) c;
		
		if (c == ',') {
			/* entonces ya leimos el MAC antes */
			buff[p] = '\0';
			p = 0;	/* reinicializamos el puntero */
			MAC = QString (buff); /* guardamos el MAC */
		} else if (c == '\n') {
			/* estamos en el caso en el que tenemos el nick antes */
			buff[p] = '\0';
			nick = QString (buff);
			
			/* agregamos el usuario si se puede, si no limpiamos
			 * los strings */
			success = udb_add_user (&MAC, &nick);
			/* reseteamos el puntero */
			p = 0;
		} else
			p++;
		
		/* leemos el proximo caracteer */
		c = fgetc(fd);
		
		/* hacemos un mini chequeo */
		if (p == 300)
			success = false;
	}
	
	/* cerramos el archivo */
	fclose (fd);
	
	return success;
}

/* Guarda en el archivo abierto al principio de todo, esto para
* evitar inconsistencias de usuarios, tener repetidos y esas
* cosas.
*/
void UDataBase::saveToFile (void)
{
	QHash<QString, CUser*>::const_iterator i;
	QString data;
	CUser * user = NULL;
	
	
	/* reabrimos el archivo para escribirlo de cero y no hacerle appends */
	if (this->file != NULL)
		fclose (this->file);
	
	this->file = fopen (qstrtochar(this->fname), "w");
	/* verificamos que el archivo este abierto */
	if (this->file == NULL){
		debugp ("UDataBase::saveToFile: error file == NULL\n");
		return;
	}
	
	
	
	/* iteramos sobre toda la hashtable */
	for (i = this->hashTable.constBegin(); 
		i != this->hashTable.constEnd(); ++i) {
		
		/* obtenemos el user */
		user = i.value();
		data = user->getMAC();
		/* tenemos: MAC */
		data.append(QChar(','));
		/* tenemos: MAC, */
		data.append (user->getNick());
		/* tenemos: MAC,Nickname */
		data.append(QChar('\n'));
		/* tenemos: MAC,Nickname\n */
		/* ahora lo guardamos en el archivo */
		fprintf (this->file, "%s", qstrtochar(data));
	}
}


/* Funcion que limpia la base de datos */
void UDataBase::clean (void)
{
	QHash<QString, CUser*>::iterator i;
	CUser * user = NULL;
	
	/*! atomico */
	this->mutex.lock();
	/* iteramos sobre toda la hashtable */
	for (i = this->hashTable.begin(); i != this->hashTable.end(); ++i) {
		
		/* obtenemos el user */
		user = i.value();
		if (user != NULL)
			delete user;
	}
	this->hashTable.clear();
	
	
	this->mutex.unlock();
}


/* Funcion que corre el servidor en determinados puertos. Esto
* lo corre en un nuevo thread, por lo que no es bloqueante.
* Esta funcion permite la posibilidad de accesos externos
* a la base de datos respetando el protocolo <SSDBP>.
* Vamos a escuchar en alguno de los puertos de rango definido
* arriba.
* NOTE: Si esta funcion es llamada y el servidor esta corriendo
* 	no tiene efecto.
*/
void UDataBase::runServer (void)
{
	/* chequeamos que no este ejecutandose */
	if (this->isRunning() || this->serverOn)
		return;
	/* si no se esta ejecutando empezamos entonces */
	this->start();
}


/* Funcion que cierra el servidor.
* NOTE: en caso de que no este corriendo, no tiene efecto.
*/
void UDataBase::stopServer (void)
{
	/* verificamos si estamos corriendo el server o no */
	if (!this->isRunning() && !this->serverOn)
		/* no esta corriendo => no frenamos nada */
		return;
	this->serverOn = false;
}

/* Funcion que practicamente carga el server y se ejecuta
* en el thread (es llamada desde runServer())
*/
void UDataBase::run (void)
{
	SServer *server = new SServer(SSDBP_MAX_BUFF_SIZE);
	SClient *client = NULL;
	unsigned short startPort = UDBSERVER_START_PORT;
	bool listening = false;
	unsigned int errCount = 0;
	int errCode = 0;
	
	
	
	/* empezamos el server */
	this->serverOn = true;
	/* intentamos escuchar */
	for (startPort = UDBSERVER_START_PORT; startPort < UDBSERVER_END_PORT &&
		!listening; startPort++)
		listening = server->startListen (startPort);
	
	/* verificamos que estemos escuchando, si no abortamos todo */
	if (!listening) {
		/*! zatanas */
		debugp ("UDataBase::run: error al intentar hacer el listeng\n");
		this->serverOn = false;
		return;
	}
	/* tamos aca es porque estamos escuchando correctamente */
	/*! ahora comenzamos a aceptar y luego trabajar con los clientes hasta
	 * que nos cierren el server.
	 */
	while (this->serverOn || errCount < 15 /* ... try 15 times :) */) {
		client = server->acceptClient();
		if (client == NULL) {
			errCount++;
			continue; /* volvemos a intentar */
		}
		/* verificamos que se registre el usuario */
		errCode = udbs_registerClient (client);
		/* ahora vamos a trabajar con el cliente */
		while (client && this->serverOn && errCode == 0) {
			/* recibimos una request valida */
			errCode = udbs_receiveReq (client);
			if (errCode == 0)
				errCode = udbs_RPCWork (client, this);
		}
		/* si estamos aca es porque se cerro el server o porque el
		 * cliente dio algun error => en cualquier caso destruimos
		 * el cliente (cierra el socket, etc) */
		delete client; client = NULL;
		/* reseteamos para que no muera el server */
		errCode = 0;
	}
	
	/* termino el server */
	server->stopServer();
	this->serverOn = false;
	delete server;
}

/*!DEBUG*/
#ifdef __DEBUG
void UDataBase::print()
{
	QHash<QString, CUser*>::const_iterator i;
	CUser * user = NULL;
	
	
	/* iteramos sobre toda la hashtable */
	for (i = this->hashTable.constBegin(); 
	i != this->hashTable.constEnd(); ++i) {
		/* obtenemos el user */
		user = i.value();
		user->printUser();
	}
}
#endif

/* NOTE: libera toda la hash table */
UDataBase::~UDataBase()
{
	/* por las dudas liberamos el mutex */
	this->mutex.unlock();
	
	/* cerramos el archivo */
	if (this->file)
		fclose (this->file);
	/* limpiamos la hashTable */
	clean ();
}