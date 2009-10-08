#include "sclient.h"


/* Constructor:
* REQUIRES:
*	maxBuffSize > 0
*	addr != NULL
*	fd >=	0 && valid(fd) == true
* maxBuffSize == el tamaño maximo del "paquete" que vamos a
* poder recibir por este cliente.
*/
SClient::SClient (struct sockaddr_in * addr, unsigned int maxBuffSize1,
	  int fd)
{
	/* PRES */
	ASSERT (addr != NULL);
	ASSERT (maxBuffSize1 > 0);
	ASSERT (fd >= 0);	/* valid fd .... */
	
	this->flags = 0;
	this->clientAddr = addr;
	this->clientfd = fd;
	this->maxBuffSize = maxBuffSize1;
	this->data = (char *) calloc (maxBuffSize1 + 2, sizeof (char));
	ASSERT (this->data != NULL);
	this->datap = 0;	/* puntero a los datos */
	
	
}
	

/*! Funcion que va a determinar si las acciones son Bloqueantes
* o no. Osea si al leer / escribir es bloqueante o no.
*/
void SClient::setWaitActions (bool wait)
{
	if (!wait)
		this->flags = MSG_DONTWAIT;
	else
		this->flags = 0;
}
/* Funcion que devuelve true si esta seteado el flag de
* "espera".
*/
bool SClient::isWaitFlag (void)
{
	return (this->flags != 0);
}



/* Funcion que empieza a recibir datos borrando el contenido
* de los datos recibidos hasta el momento (clearBuffer())
* NOTE: Es bloqueante esta funcion.
* RETURNS:
*	bytes leidos (> 0)
*	< 0 en caso de error
*	== 0 en caso de cierre de conexion
*/
int SClient::readData(void)
{
	int result = 0;
	
	/* limpiamos el buffer */
	clearBuffer();
	
	/* vamos a intentar leer */	
	result = recv (this->clientfd, this->data, this->maxBuffSize,
			this->flags);
	
	/* actualizamos el puntero */
	if (result >= 0)
		datap += result;
	
	return result;
}
	  
/* Funcion que empieza a recibir datos sin borrar el contenido
* anterior del buffer (hace un data.append()).
* NOTE: Es bloqueante esta funcion.
* RETURNS:
*	bytes leidos (> 0)
*	< 0 en caso de error
*	== 0 en caso de cierre de conexion
*/
int SClient::continueReading(void)
{
	int result = 0;
	
	
	/* vamos a intentar leer */	
	result = recv (this->clientfd, &(this->data[datap]),
			this->maxBuffSize - datap, this->flags);
			
	/* actualizamos el puntero */
	if (result >= 0)
		datap += result;
	
	return result;
}

/* Funcion que obtiene el buffer de datos leidos
* RETURNS:
*	const data != NULL
*/
const char * SClient::getData(void)
{	
	return this->data;
}


/* Funcion que devuelve el tamaño del data.
* RETURNS:
*	0 	si no hay datos
*	size	otherwise
*/
int SClient::getDataSize (void)
{
	return this->datap;
}

/* Funcion que limpia el buffer de recepcion
* (data.clear())
*/
void SClient::clearBuffer (void)
{
	memset (this->data, 0, this->maxBuffSize + 1);
	this->datap = 0;
}


/* Funcion que devulve el estado del buffer de entrada. Si esta
* lleno o no.
* RETURNS:
*	true	if readBuffer its full
*	false	otherwise
*/
bool SClient::isBuffFull(void)
{
	 return (this->datap == this->maxBuffSize);
}


/* Funcion que envia datos por medio del socket.
* REQUIRES:
*	data != NULL
* RETURNS:
*	errCode (<= 0 en caso de error)
*	bytes sended
* NOTE: es bloqueante y se bloquea hasta que TODOS los datos
*	fueron enviados.
*/
int SClient::sendData (const char * info, int size)
{
	int result = 0;
	int bs = 1;
	int bytesToSend = size;
	
	
	/* pres */
	ASSERT (data != NULL);
	ASSERT (size >= 0);
	
	if (this->flags != 0) {
		result = send (this->clientfd, &info[size-bytesToSend],
				bytesToSend, this->flags);
		return result;
	}
	
	/*! tenemos que mandar todo y bloqueante */
	/* ahora mientras tengamos que enviar vamos a intentar mandar
	 * todo de una... y nos vamos a bloquear hasta que se mande todo */
	while (bs > 0 && bytesToSend > 0) {
		bs = send (this->clientfd, &info[size - bytesToSend],
				bytesToSend, this->flags);
		bytesToSend -= bs;
		result += bs;
	}
	
	/* hay un error */
	if (bs < 0)
		result = bs;
	
	return result;
}

/* Funcion que devuelve el file descriptor actual del cliente
*/
int SClient::getFd(void)
{
	return this->clientfd;
}

/* Funcion que devuelve el sockaddr_in del cliente, Por si 
* se quiere obtener el puerto/ip/lo que sea.
*/
struct sockaddr_in * SClient::getAddr(void)
{
	return this->clientAddr;
}


/* Destructor, cierra todo */
SClient::~SClient()
{
	/* borramos todo */
	if (this->data != NULL)
		free (this->data);
	if (this->clientAddr != NULL)
		free (this->clientAddr);
	
	/* cerramos el fd por las dudas */
	close (this->clientfd);
}