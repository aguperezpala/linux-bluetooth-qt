/*! Esta clase practicamente representa un servidor simple que escucha en un
 * determinado puerto y lo que hace posible es recibir datos hasta cierto
 * tamaño determinado al momento de la construccion. En caso de exceder este
 * tamaño maximo, devuelve un error y limpia el buffer de lectura.
 */
#ifndef SSERVER_H
#define SSERVER_H

/* librerias generales */
#include <stdio.h>	
#include <stdlib.h>
#include <string.h>
/* sockets */
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
/* alto nivel */
#include <QString>

/* libs propias */
#include "../consts.h"
#include "../debug.h"

/* cantidad de conexiones simultaneas soportadas por el servidor (1 por ahora)*/
#define SSERVER_CONNECTIONS_LIMIT	1

class SServer:{
	public:
		/* Constructor:
		 * REQUIRES:
		 *	maxBuffSize > 0
		 * maxBuffSize == el tamaño maximo del "paquete" que vamos a
		 * poder recibir por este servidor
		 */
		SServer (unsigned int maxBuffSize);
		
		/* Funcion que escucha en un puerto determinado:
		 * REQUIRES:
		 *	port >= 0
		 * RETURNS:
		 *	true 	if success
		 *	false	otherwise
		 * NOTE: hace el bind tambien y el accept, es BLOQUEANTE.
		 */
		bool startListen (unsigned short port);
		
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
		 *	const data
		 * NOTE: data.isNull() == true (si no hay datos)
		 */
		const QString & getData(void);
		
		/* Funcion que limpia el buffer de recepcion
		 * (data.clear())
		*/
		void clearBuffer (void);
		
		/* Funcion que detiene el servidor */
		void stop(void);
		
		/* Destructor, cierra todo */
		~UServer();
		
	private:	
		
		unsigned short port;	/* puerto en el que escuchamos */
		bool running;		/* para determinar cuando pararlo */
		int sock;		/* socket donde escuchamos */
		QString data;		/* donde recibimos los datos */
		unsigned int maxBuffSize;/* tamaño maximo del buffer */
		int clientfd;	/* socket del cliente */
		
};

#endif