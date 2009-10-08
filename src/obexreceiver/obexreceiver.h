/*! Modulo que se va a encargar de recibir lo que nos manda el obexserver.
 * Vamos a recibirlo via LAN para hacerlo mas generico al protocolo (y rapido).
 */
#ifndef OBEXRECEIVER_H
#define OBEXRECEIVER_H


#include <QString>
#include <QByteArray>
#include <QFile>

#include "../consts.h"
#include "../debug.h"
#include "obexparser/obexparser.h"
#include "../simple_server/sserver.h"
#include "../simple_server/sclient.h"
#include "../dispobject/dispobject.h"
#include "../udatabase/udatabase.h"
#include "../cuser/cuser.h"


class ObexReceiver {
	public:
		/* Constructor
		 * Inicializa las variables del servidor
		 * NOTE: Requiere una base de datos para funcionar.
		 * REQUIRES:
		 *	udb != NULL
		*/
		ObexReceiver(UDataBase * udb);
		
		
		/* Funcion que inicializa el server y se bloquea hasta que		
		 * algun cliente se conecta.
		 * RETURNS;
		 *	true 	on success
		 *	false	on error
		 * NOTE: ES BLOQUEANTE
		*/
		bool startReceiver(void);
		
		
		/* Funcion que sirve para reiniciar el server, en caso de que
		 * este abierto, esta funcion lo cierra y comienza de nuevo
		 * a escuchar.
		 * Es una funcion de fuerza "bruta"... no se recomienda :S
		 */
		void resetServer (void);
		
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
		 * NOTE: si hay un error interno es probable que haya que
		 *	reiniciar el server (resetServer ;( ).
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
		UDataBase * db;
		
		
		/* Funcion que genera un DispObject desde una QStringList,
		 * chequeando que respete algunas de las condiciones que
		 * pedimos.
		 * REQUIRES:
		 *	strList != NULL
		 * RETURNS:
		 *	NULL	on error (no tenemos los datos que requerimos
		 *			  osea que no respeta el protocolo)
		 *	dobj	on success (NOTE: Alloca memoria)
		 */
		DispObject * dispObjFromStrList (QStringList * strList);
		 
};

#endif