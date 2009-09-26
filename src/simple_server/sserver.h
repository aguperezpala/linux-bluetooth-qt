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
#include <unistd.h>


/* libs propias */
#include "sclient.h"
#include "../consts.h"
#include "../debug.h"



/* cantidad de conexiones simultaneas soportadas por el servidor */
#define SSERVER_CONNECTIONS_LIMIT	50

class SServer {
	public:
		/* Constructor:
		 * REQUIRES:
		 *	maxBuffSize > 0
		 * maxBuffSize == el tamaño maximo del "paquete" que vamos a
		 * poder recibir por este servidor (es heredado hacia las
		 * instancias de los SClient's.
		 */
		SServer (unsigned int maxBuffSize);
		
		/* Funcion que escucha en un puerto determinado:
		 * REQUIRES:
		 *	port >= 0
		 * RETURNS:
		 *	true 	if success
		 *	false	otherwise
		 * NOTE: hace el bind tambien. ES BLOQUEANTE
		 */
		bool startListen (unsigned short port);
		
		/* Funcion que accepta conexiones, devuelve una nueva instancia
		 * de SClient * si es que se se establecio una nueva conexion.
		 * Toma el primero que este en la cola de conexiones.
		 * NOTE: ES BLOQUEANTE.
		 * RETURNS:
		 *	SClient * != NULL (nuevo cliente)
		 *	NULL	on error
		 * NOTE: el SClient devuelto es propiedad del que lo obtiene.
		 *	el servidor no se encarga de liberar estos clientes.
		 */
		SClient * acceptClient(void);
		
		/* Funcion que devuelve el puerto en el que estamos escuchando
		 */
		inline unsigned short getPort(void) {return this->port;};
		
		/* Funcion que devuelve el sockfd */
		inline int getSocket (void) {return this->sock;};
		
		/* Funcion que cierra el servidor (close(sock))
		*/
		void stopServer();
		
		/* Destructor, cierra todo */
		~SServer();
		
	private:	
		
		unsigned short port;	/* puerto en el que escuchamos */	
		int sock;		/* socket donde escuchamos */
		unsigned int maxBuffSize;/* tamaño maximo del buffer */
		
		
};

#endif