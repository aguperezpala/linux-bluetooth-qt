#ifndef BTSYSTEMMANAGER_H
#define BTSYSTEMMANAGER_H

#include <string>
#include <list>
#include <stdio.h>
#include <assert.h>
#include <QMutex>
#include <QThread>
#include <QList>
/* Libs de bluetooth propias */
#include <bt-manager/btdmanager.h>
#include <bt-manager/btdongledevice.h>
#include <bt-manager/btconnection.h>

#include "btreceiver.h"
#include "udatabase.h"
#include "btobjgenerator.h"
#include "btprotocol.h"


using namespace::std;

/* declaramos un enum para determinar el estado del sistema */
enum {
	BTSM_INITIALIZED,	/* sistema inicializado */
	BTSM_RUNNING,		/* sistema corriendo correctamente */
	BTSM_STOP,		/* sistema parado */
	BTSM_ERROR		/* error */
};

class BTSystemManager : public QThread {
	public:
		/* constructor:
		 * REQUIRES:
		 * 	udb != NULL
		 * 	load = (true => load codes from file, false => new one)
		 */
		BTSystemManager(UDataBase *udb, bool load = false);
		
		/* Funcion que devuelve un reporte de la ultima accion
		 * realizada. */
		QString &getReport(void);
		
		/* Funcion que verifica los dongles del sistema y los inicializa
		 * RETURNS:
		 * 	< 0	si se encontro algun fallo
		 * 	0	si no hubo fallos.
		 * NOTE: deja en report el reporte obtenido.
		 */
		int initializeDongles(void);
		
		/* Funcion que intenta reparar los dongles del sistema 
		 * dandole de baja o haciendo lo necesario para intentar hacer
		 * que funcione.
		 * RETURNS:
		 * 	< 0	on error
		 * 	0	if succes
		 * NOTE: deja en report los resultados
		 * FIXME
		 */
		int tryToRepairDongles(void);
		
		/* Funcion que determina el tiempo maximo que puede estar
		 * una conexion activa en usecs. */
		void setMaxConnTime(long m){this->maxConnTime = m;};
		long getMaxConnTime(void){return this->maxConnTime;};
			
		
		 
		
		/*! Funcion que Ejecuta el BTReceiver y comienza a recibir.
		 * Ademas chequea las conexiones (toda aquella conexion que
		 * este mas de n msecs sera desconectada automaticamente)
		 * NOTE: Corre en un thread
		 * REQUIRES:
		 * 	status != (ERROR | RUNNING)
		 * RETURNS:
		 * 	< 0	on error
		 * 	0	if success
		 */
		int startToReceive(void);
		void run(void);
		
		
		/* Funcion que detiene el BTReceiver y termina el thread. 
		*/
		void stopReceiving(void);
		
		/*! Funcion que devuelve un elemento de la lista si es que hay,
		 * caso contrario se bloquea la llamada hasta que exista un
		 * elemento.
		 * RETURNS:
		 * 	dObj
		 */
		DispObject *getDispObject(void);
		
		/* Funcion que devuelve la cantidad de de DispObjects 
		 * disponibles */
		int getNumDispObjs(void);
		
		/* Funcion que devuelve el estado en el que se encuentra el
		 * sistema.*/
		int getStatus(void){return this->status;};
		
		
		/* destructor
		 * NOTE: libera el btReceiver y el BtObjGen 
		 * */
		~BTSystemManager(void);
		
	
	
	private:
		/*			FUNCIONES			*/
		
		/* funcion que setea un string como error y cambia el estado
		 * a algun estado determinado.
		 * REQUIRES:
		 * 	msg 	!= NULL
		 */
		void setMsgAndState(const char *msg, int state);
		
		/* Funcion que va a cerrar toda aquella conexion que exceda 
		 * maxConnTime.
		 * REQUIRES:
		 * 	list<BTConnection *>
		 * RETURNS:
		 * 	n > 0	number of connection closed
		 * 	< 0	on error
		 */
		int closeConnTimeExceeded(const list<BTConnection *> &list);
		
		
		
		/*	Atributos	*/
		bool canReceive;
		long maxConnTime;
		BTReceiver *btRec;
		BTObjGenerator *btObjGen;
		UDataBase *udb;
		QString report;		/* reporte de la ultima accion realizada */
		int status;
		QMutex dispMutex;
		QMutex auxMutex;
		QList<DispObject *> dobjsList;
	
};

#endif
