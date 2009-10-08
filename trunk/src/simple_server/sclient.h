/*! Esta clase practicamente representa a un cliente determinado, luego de haber
 *  hecho un accept desde un servidor especifico. Mantiene el fd (socket), y
 *  permite enviar datos y recibirlos.
 *  El buffer maximo va a estar determinado por el server practicamente.
 */
#ifndef SCLIENT_H
#define SCLIENT_H

/* librerias generales */
#include <stdio.h>	
#include <stdlib.h>
#include <string.h>
/* sockets */
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <unistd.h>



/* libs propias */
#include "../consts.h"
#include "../debug.h"


class SClient{
	public:
		/* Constructor:
		 * REQUIRES:
		 *	maxBuffSize > 0
		 *	addr != NULL
		 *	fd >=	0 && valid(fd) == true
		 * maxBuffSize == el tamaño maximo del "paquete" que vamos a
		 * poder recibir por este cliente.
		 * NOTE: No se debe liberar addr despues de haber llamado a esta
		 *	funcion.
		 */
		SClient (struct sockaddr_in * addr, unsigned int maxBuffSize,
			  int fd);
		
		/*! ### ### ### <<<UNIMEPLEMENTED>>>	### ### ###	*/
		/*! Funcion que va a determinar si las acciones son Bloqueantes
		 * o no. Osea si al leer / escribir es bloqueante o no.
		 */
		void setWaitActions (bool wait);
		
		/* Funcion que devuelve true si esta seteado el flag de
		 * "espera".
		 */
		bool isWaitFlag (void);
		
		/*! ### ### ### <<<UNIMEPLEMENTED>>>	### ### ###	*/
		
		/* Funcion que empieza a recibir datos borrando el contenido
		 * de los datos recibidos hasta el momento (clearBuffer())
		 * NOTE: Es bloqueante esta funcion.
		 * RETURNS:
		 *	bytes leidos (> 0)
		 *	< 0 en caso de error
		 *	== 0 en caso de cierre de conexion
		 */
		int readData(void);
		
		/* Funcion que empieza a recibir datos sin borrar el contenido
		* anterior del buffer (hace un data.append()).
		* NOTE: Es bloqueante esta funcion.
		* RETURNS:
		*	bytes leidos (> 0)
		*	< 0 en caso de error
		*	== 0 en caso de cierre de conexion
		*/
		int continueReading(void);
		
		/* Funcion que obtiene el buffer de datos leidos
		 * RETURNS:
		 *	const data != NULL
		 * NOTE: data tiene un size determinado, se obtiene con
		 *	getDataSize()
		 */
		const char * getData(void);
		
		/* Funcion que devuelve el tamaño del data.
		 * RETURNS:
		 *	0 	si no hay datos
		 *	size	otherwise
		 */
		int getDataSize (void);
		
		/* Funcion que limpia el buffer de recepcion
		 * (data.clear())
		*/
		void clearBuffer (void);
		
		
		/* Funcion que devulve el estado del buffer de entrada. Si esta
		 * lleno o no.
		 * RETURNS:
		 *	true	if readBuffer its full
		 *	false	otherwise
		 */
		bool isBuffFull(void);
		
		/* Funcion que envia datos por medio del socket.
		 * REQUIRES:
		 *	data != NULL
		 *	size >= 0
		 * RETURNS:
		 *	errCode (<= 0 en caso de error)
		 *	bytes sended
		 * NOTE: es bloqueante y se bloquea hasta que TODOS los datos
		 *	fueron enviados.
		 */
		int sendData (const char * data, int size);
		
		/* Funcion que devuelve el file descriptor actual del cliente
		*/
		int getFd(void);
		
		/* Funcion que devuelve el sockaddr_in del cliente, Por si 
		 * se quiere obtener el puerto/ip/lo que sea.
		 */
		struct sockaddr_in * getAddr(void);
		
		
		/* Destructor, cierra todo */
		~SClient();
		
	private:	
		struct sockaddr_in *clientAddr;	/* guardamos la direccion */
		int datap;		/* data pointer (donde recibimos) */
		char *data;		/* donde recibimos los datos */
		unsigned int maxBuffSize;/* tamaño maximo del buffer */
		int clientfd;		/* socket del cliente */
		int flags;
		
};

#endif