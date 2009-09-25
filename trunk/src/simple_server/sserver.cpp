#include "userver.h"

/* Funcion que trabaja sobre un cliente especifico, lo que hace
* es leer hasta recibir un pedido determinado, una vez que
* recibe, verifica si el cliente se registro, si re registro
* entonces comienza a recibir datos y responderlos, en caso
* de recibir alguna especie de error de protocolo la funcion
* vuelve con un codigo de error que produce el cierre de la
* conexion.
* REQUIRES:
*	clientfd >= 0	(valido)
* RETURNS:
*	< 0 	on error
*	0 	if NO error
*/
int UServer::workClient(int clientfd)
{
	int errCode = 0;
	bool userRegistered = false;
	QString req = "";
	QString resp = "";
	char buff[USP_SSAP_MAZ_SIZE+1] = {0};
	int bytesReaded = 0;
	int dummy = 0;
	CUser * user = NULL;
	
	
	/*! El ciclo de trabajo va a ser practicamente esto:
	 * 1) Recibimos hasta que el cliente se registre (clientLoged = true).
	 * 1.a) Enviamos un "ok".
	 * 2) Si recibimos un "register" de nuevo => devolvemos error.
	 * 3) Recibimos todo el tiempo (paquetes del tipo (MAC,Nick).
	 * 3.a) Si hay algun error en estos packetes => devolvemos error.
	 * 3.b) Si esta todo ok, enviamos un "ok".
	 */
	
	/* primero que todo vamos a cumplir 1) */
	while (errCode == 0 && !userRegistered) {
		/* leemos */
		bytesReaded = read (clientfd, buff, USP_SSAP_MAZ_SIZE);
		if (bytesReaded <= 0) {
			debugp("UServer::workClient: Error al intentar leer "
			"el clientefd. No se pudo registrar ademas.\n");
			errCode = -1;
			continue;
		}
		buff[bytesReaded] = '\0';
		/* concatenamos lo que recibimos */
		req.append(buff);
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
					dummy = write(clientfd,
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
	/*! si no esta registrado es porque ta al hornapio esto y es error */
	if (!userRegistered)
		return errCode; /*! ASSERT (errCode < 0); */
	
	/*! aca estamos con el usuario registrado y esperando a seguir
	 * recibiendo datos. Ahora solo del tipo <SSAP>MAC,Nick<SSAP> */
	
	while (errCode == 0) {
		/* leemos */
		bytesReaded = read (clientfd, buff, USP_SSAP_MAZ_SIZE);
		if (bytesReaded <= 0) {
			debugp("UServer::workClient: Error al intentar leer "
			"el clientefd. No se pudo registrar ademas.\n");
			errCode = -1;
			continue;
		}
		buff[bytesReaded] = '\0';
		/* concatenamos lo que recibimos */
		req.append(buff);
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
						 write (clientfd,
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

/* Constructor:
* REQUIRES:
*	maxBuffSize > 0
* maxBuffSize == el tamaño maximo del "paquete" que vamos a
* poder recibir por este servidor
*/
SServer::SServer (unsigned int mbs)
{
	ASSERT (maxBuffSize > 0);
	this->maxBuffSize = mbs;
}

/* Funcion que escucha en un puerto determinado:
* REQUIRES:
*	port >= 0
* RETURNS:
*	true 	if success
*	false	otherwise
* NOTE: hace el bind tambien, es BLOQUEANTE.
*/
bool SServer::startListen (unsigned short port)
{
	struct sockaddr_in self;
	int clientfd = 0;
	struct sockaddr_in clientAddr;
	socklen_t addrlen = sizeof (clientAddr);
	
	
	this->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sock < 0) {
		debugp ("SServer::startListen: Error al crear el socket\n");
		return false;
	}
	debugp ("SServer::listen: socket creado correctamente\n");
	
	self.sin_family = AF_INET;
	self.sin_addr.s_addr = INADDR_ANY;
	self.sin_port = htons(port);
	
	/* hacemos el bind */
	if (bind(this->sock, (struct sockaddr*)&self, sizeof(self)) != 0 ) {
		debugp ("SServer::startListen: Error al hacer el bind\n");
		close (this->sock);
		return false;
	}
	
	/* escuchamos */
	if (listen (this->sock, SSERVER_CONNECTIONS_LIMIT) != 0 ) {
		debugp ("SServer::startListen: Error al hacer el listen\n");
		close (this->sock);
		return false;
	}
	debugp ("SServer::startListen: escuchando\n");
	this->port = port;
	
	/* devolvemos */
	return true;
}

/* Esta es la funcion mas importante de todas practicamente,
* lo que hace es escuchar en determinado puerto y a toda
* conexion entrante la acepta y comienza a recibir los datos.
* Solo terminara de correr cuando se llame a server.stop();
*/
void UServer::run(void)
{
	int i = 0;
	bool listening = false, error = false;
	int clientfd = 0;
	struct sockaddr_in clientAddr;
	socklen_t addrlen = sizeof (clientAddr);
	
	/* primero que todo intentamos ponernos a la escucha dentro del rango
	 * de puertos */
	for (i = this->startPort; i <= this->endPort && !listening; i++)
		if (doListen ((unsigned int) i) >= 0)
			/* estamos escuchando en el puerto i */
			listening = true;
		
	/* hacemos una verificacion */
	if (i > this->endPort && !listening) {
		debugp ("UServer::run: error al intentar escuchar\n");
		return; /*! salimos, no podemo hace nada loco */
	}
	
	this->running = true;	/* estamos corriendo */
	
	while (this->running && !error) {
		/* aceptamos de a una conexion */
		clientfd = accept (this->sock, (sockaddr*) &clientAddr, 
				    &addrlen);
		/*! si queremos info del cliente la tenemos en clientAddr */
		if (clientfd < 0) {
			debugp ("UServer::run: Error acpetando una conexion\n");
			error = true;
			continue; /* salteamos el while */
		}
		debugp ("UServer::run: Aceptamos un nuevo cliente\n");
		if (workClient (clientfd) < 0) {
			/* cerramos la conexion, esperamos aceptar otra */
			debugp ("UServer::run: Cerrando client conection\n");
			clientfd = close (clientfd);
		}
	}
	
	/* cerramos la conexion del socket que estamos escuchando */
	this->sock = close (this->sock);
}


/* Funcion que detiene el servidor */
void UServer::stop(void)
{
	this->running = false;
	/* force close XD */
	if (this->sock)
		this->sock = close (this->sock);
}


/* Destructor, cierra todo */
UServer::~UServer()
{
	/* do nothing */
	return;
}