/*! Modulo que se va a encargar de recibir lo que nos manda el obexserver.
 * Vamos a recibirlo via LAN para hacerlo mas generico al protocolo (y rapido).
 */
#ifndef OBEXRECEIVER_H
#define OBEXRECEIVER_H


#include <QString>
#include <QBytesArray>

#include "../consts.h"
#include "../debug.h"
#include "../simple_server/sserver.h"
#include "../simple_server/sclient.h"
#include "../dispobject/dispobject.h"


class ObexReceiver {
	public:
		/* Constructor
		 * Inicializa las variables del servidor
		*/
		ObexReceiver(void);
		
		
		/* Funcion que inicializa el server y se bloquea hasta que		
		 * algun cliente se conecta.
		 * NOTE: ES BLOQUEANTE
		*/
		void startReceiver(void);
		
		/* Funcion que devuelve un DispObject *. Lo que hace es leer
		 * del buffer si tenemos suficiente info como para armar un
		 * DispObject. Si es asi devolvemos automaticamente y eliminamos
		 * del buffer lo obtenido.
		 * Si no tenemos suficiente info => leemos del socket eso lo
		 * hace ### BLOQUEANTE ###
		 * Ademas suprime todo tipo de error, sigue escuchando.
		 * RETURNS:
		 * 	dobj 	!= NULL (si todo andubo bien, casi siempre)
		 * 	NULL 	si hubo error interno (poco probable).
		 *//*! Esta es la funcion principal, hace todo practicamente */
		DispObject * getDispObj (void);
		
		
		/* Destructor.
		 * Elimina el cliente, los datos, y el servidor, cierra tambien
		 * la conexion.
		*/
		~ObexReceiver();
		
	private:
		SServer *server;
		SClient *client;
		QByteArray buff;
		
		/* Funcion que recibe los datos y los concatena en el buffer 
		 * NOTE: es bloqueante
		*/
		void readDevice(void);
		
		
		
};

#endif