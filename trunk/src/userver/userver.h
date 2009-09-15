/*! Esta clase va a ser la encargada de establecer las conexiones con los 
 * UserRegisterPrograms, para recibir los usuarios via lan. Los agrega
 * a la base de datos UDataBase (requiere de esta clase para su existencia
*/
#ifndef USERVER_H
#define USERVER_H

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
#include <QThread>


#include "usparser.h"
#include "../udatabase/udatabase.h"
#include "../cuser/cuser.h"
#include "../consts.h"
#include "../debug.h"

/* cantidad de conexiones simultaneas soportadas por el servidor (1 por ahora)*/
#define USERVER_CONNECTIONS_LIMIT	1

class UServer: public QThread {
	public:
		/* Constructor: Va a pedir la UDataBaser, ya que de esta depende
		 * para su existencia. Ademas se va a pedir un rango de puertos
		 * para que escuche, para hacerlo mas robusto.
		 * REQUIRES:
		 *	udb != NULL
		 *	endPort >= startPort
		 */
		UServer (UDataBase * udb, int startPort, int endPort);
		
		/* Esta es la funcion mas importante de todas practicamente,
		 * lo que hace es escuchar en determinado puerto y a toda
		 * conexion entrante la acepta y comienza a recibir los datos.
		 * Solo terminara de correr cuando se llame a server.stop();
		 *//*! NOTE: Se debe llamar como server.start() (por el thread)
		 */
		void run(void); 
		
		
		/* Funcion que detiene el servidor */
		void stop(void);
			
		
		/* Destructor, cierra todo */
		~UServer();
		
	private:	
		UDataBase * udb;	/* base de datos del usuario */
		USParser parser;	/* parser del servidor */
		/* rango de puertos */
		int startPort;
		int endPort;
		bool running;		/* para determinar cuando pararlo */
		int sock;		/* socket donde escuchamos */
		
		
		/*! funciones auxiliares */
		
		/* Funcion que escucha en determinado puerto, hasta que alguien
		 *se conecta.
		 * RETURNS:
		 * 	fd
		 */
		int doListen(unsigned int port);
		
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
		int workClient(int clientfd);
		
		
};

#endif