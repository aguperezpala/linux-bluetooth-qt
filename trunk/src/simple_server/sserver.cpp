#include "sserver.h"

/* Constructor:
* REQUIRES:
*	maxBuffSize > 0
* maxBuffSize == el tamaño maximo del "paquete" que vamos a
* poder recibir por este servidor
*/
SServer::SServer (unsigned int mbs)
{
	ASSERT (mbs > 0);
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




/* Funcion que accepta conexiones, devuelve una nueva instancia
* de SClient * si es que se se establecio una nueva conexion.
* Toma el primero que este en la cola de conexiones.
* NOTE: ES BLOQUEANTE.
* RETURNS:
*	SClient * != NULL (nuevo cliente)
* NOTE: el SClient devuelto es propiedad del que lo obtiene.
*	el servidor no se encarga de liberar estos clientes.
*/
SClient * SServer::acceptClient(void)
{
	SClient * client = NULL;
	struct sockaddr_in *clientAddr = NULL;
	socklen_t addrlen = sizeof (sockaddr_in);
	int clientfd = 0;
	
	
	/* allocamos espacio para la clientAddr */
	clientAddr = (sockaddr_in *) calloc (1, sizeof (sockaddr_in));
	ASSERT (clientAddr != NULL);
	
	/* ahora vamos hacer un simple accept, y nos vamos a "bloquear"
	 * hasta que algun cliente se conecte practicamente. */
	clientfd = accept (this->sock, (sockaddr*) clientAddr, &addrlen);
	/*! si queremos info del cliente la tenemos en clientAddr */
	if (clientfd < 0) {
		debugp ("UServer::acceptClient: Error acpetando conexion\n");
		/* limpiamos la memoria :( */
		free (clientAddr);
		return client;	/* devolvemos null */
	}
	
	/*! si estamos aca es porque pudimos aceptar correctamente =>
	 * entonces vamos a devolver el cliente */
	client = new SClient (clientAddr, this->maxBuffSize, clientfd);
	
	return client;
}

/* Funcion que cierra el servidor (close(sock))
*/
void SServer::stopServer()
{
	if (this->sock)
		close (this->sock);
}
/* Destructor, cierra todo */
SServer::~SServer()
{
	/* cerramos el socket */
	if (this->sock)
		close (this->sock);
}