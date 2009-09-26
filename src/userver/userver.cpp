#include "userver.h"


/* Funcion que trabaja sobre un cliente especifico, lo que hace
* es leer hasta recibir un pedido determinado, una vez que
* recibe, verifica si el cliente se registro, si re registro
* entonces comienza a recibir datos y responderlos, en caso
* de recibir alguna especie de error de protocolo la funcion
* vuelve con un codigo de error que produce el cierre de la
* conexion.
* REQUIRES:
*	client != NULL
* RETURNS:
*	< 0 	on error
*	0 	if NO error
*/
int UServer::workClient(SClient * client)
{
	int errCode = 0;
	bool userRegistered = false;
	QString req = "";
	QString resp = "";
	char buff[USP_SSAP_MAZ_SIZE+1] = {0};
	int bytesReaded = 0;
	int dummy = 0;
	CUser * user = NULL;
	
	
	/* PRE */
	ASSERT (client != NULL);
	if (client == NULL)
		return -1;	/* error */
	
	/*! El ciclo de trabajo va a ser practicamente esto:
	 * 1) Recibimos hasta que el cliente se registre (clientLoged = true).
	 * 1.a) Enviamos un "ok".
	 * 2) Si recibimos un "register" de nuevo => devolvemos error.
	 * 3) Recibimos todo el tiempo (paquetes del tipo (MAC,Nick).
	 * 3.a) Si hay algun error en estos packetes => devolvemos error.
	 * 3.b) Si esta todo ok, enviamos un "ok".
	 */
	
	/* limpiamos un cacho */
	client->clearBuffer();
	/* primero que todo vamos a cumplir 1) */
	while (errCode == 0 && !userRegistered) {
		/* leemos */
		bytesReaded = client->continueReading();
		if (bytesReaded <= 0) {
			debugp("UServer::workClient: Error al intentar leer "
			"el cliente. No se pudo registrar ademas.\n");
			errCode = -1;
			continue;
		}
		/* verificamos lo que leimos */
		strncpy (buff, client->getData(), client->getDataSize());
		buff[client->getDataSize()] = '\0';
		req = buff;
		/* verificamos que este completo con el parser */
		switch (this->parser.isValidRequest(req)) {
			case -1: /* esta incompleto, seguimos recibiendo */
				break;
				
			case -2: /* tiene algun error => salimos */
				errCode = -1;
				break;
				
			case 0: /* recibimos un pakete y no esta registrado */
				errCode = -1;
				break;
				
			case 1: /* esto es lo que queremos recibir */
				/* resgistrado el usuario correctamente :) */
				userRegistered = true;
				/* le mandamos una respuesta :) */
				resp = "ok";
				if (this->parser.createResponse (resp))
					dummy = client->sendData(
							qstrtochar(resp),
							  resp.length());
				else
					/* no se pudo crear */
					errCode = -1;
				break;
			default:
				/* zatanas esta entre nosotros. */
				errCode = -666;
		}
	}
	
	/* limpiamos las cosas un poco */
	req = "";
	resp = "";
	memset (buff, '\0', USP_SSAP_MAZ_SIZE);
	client->clearBuffer();
	/*! si no esta registrado es porque ta al hornapio esto y es error */
	if (!userRegistered)
		return errCode; /*! ASSERT (errCode < 0); */
	
	/*! aca estamos con el usuario registrado y esperando a seguir
	 * recibiendo datos. Ahora solo del tipo <SSAP>MAC,Nick<SSAP> */
	
	while (errCode == 0) {
		/* leemos */
		bytesReaded = client->continueReading();
		if (bytesReaded <= 0) {
			debugp("UServer::workClient: Error al intentar leer "
			"el cliente. No se pudo registrar ademas.\n");
			errCode = -1;
			continue;
		}
		/* obtenemos los datos */
		strncpy (buff, client->getData(), client->getDataSize());
		buff[client->getDataSize()] = '\0';
		req = buff;
		/* verificamos que este completo con el parser */
		switch (this->parser.isValidRequest(req)) {
			case -1: /* esta incompleto, seguimos recibiendo */
				break;
			
			case 0: /* recibimos un pakete, es lo que queremos */
				user = parser.parseRequest(req);
				if (user == NULL)
					errCode = -1;
				else {
					/* agregamos el usuario a la udb */
					this->udb->addUser (user);
					/* mandamos el "ok" */
					resp = "ok";
					if (this->parser.createResponse (resp))
						 client->sendData(
						       qstrtochar(resp),
							resp.length());
					 else
						 /* no se pudo crear */		
						 errCode = -1;
				}
				/* limpiamos el request */
				req = "";
				break;
			
			default:
				/* cualquier otro caso es error */
				errCode = -1;
		}
		
	}
	
	return errCode;
}

/* Constructor: Va a pedir la UDataBaser, ya que de esta depende
* para su existencia. Ademas se va a pedir un rango de puertos
* para que escuche, para hacerlo mas robusto.
* REQUIRES:
*	udb != NULL
*	endPort >= startPort
*/
UServer::UServer (UDataBase * udb, unsigned short sPort, unsigned short ePort)
{
	/* Pres */
	ASSERT (udb != NULL);
	ASSERT (ePort >= sPort);
	
	if (udb == NULL) {
		printf ("UServer: UDataBase null al crear el servidor\n");
		exit (1);
	}
	/* hacemos unos arreglos minimos */
	if (sPort > ePort)
		ePort = sPort;
	
	/* le pasamos el tamaño maximo que soporta el SSAP protocol */
	this->server = NULL;
	this->server = new SServer (USP_SSAP_MAZ_SIZE); 
	ASSERT (this->server != NULL);
	
	this->udb = udb;
	this->startPort = sPort;
	this->endPort = ePort;
	this->running = false;
}

void UServer::startServer (void)
{
	/* ejecutamos el thread */
	this->start();
}
/* Esta es la funcion mas importante de todas practicamente,
* lo que hace es escuchar en determinado puerto y a toda
* conexion entrante la acepta y comienza a recibir los datos.
* Solo terminara de correr cuando se llame a server.stop();
*/
void UServer::run(void)
{
	unsigned short int i = 0;
	bool listening = false, error = false;
	SClient * client = NULL;
	
	
	/* primero que todo intentamos ponernos a la escucha dentro del rango
	 * de puertos */
	for (i = this->startPort; i <= this->endPort && !listening; i++)
		if (server->startListen (i))
			/* estamos escuchando en el puerto i */
			listening = true;
		
	/* hacemos una verificacion */
	if (!listening) {
		debugp ("UServer::run: error al intentar escuchar\n");
		return; /*! salimos, no podemo hace nada loco */
	}
	
	this->running = true;	/* estamos corriendo */
	
	/* ahora viene el tema de aceptar clientes y eso, por el momento solo
	 * vamos a querer un solo cliente. Para ampliar esto lo que se deberia
	 * hacer es practicamente tener una QList de clientes y sus respectivos
	 * fd, asi por medio de select se multiplexa la recepcion.. 
	 */
	while (this->running && ! error) {
		client = server->acceptClient();
		if (client == NULL) {
			debugp ("UServer::run: error al acceptar un cliente\n");
			error = true;
			continue;
		}
		debugp ("UServer::run: Aceptamos un nuevo cliente\n");
		
		/* ahora tenemos el cliente con el que queremos trabajar */
		
		if (workClient(client) < 0) {
			debugp ("UServer::run: Cerrando client conection\n");
			/* borramos y cerramos el cliente */
			delete client; client = NULL;
		}
	}
	
	
}


/* Funcion que detiene el servidor */
void UServer::stop(void)
{
	this->running = false;
	/* force close XD */
	this->server->stopServer();
}


/* Destructor, cierra todo */
UServer::~UServer()
{
	if (this->server)
		delete this->server;
	/* do nothing */
	return;
}