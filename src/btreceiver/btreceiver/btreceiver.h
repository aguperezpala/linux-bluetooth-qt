#ifndef BTRECEIVER_H
#define BTRECEIVER_H


#include <iostream>
#include <string>
#include <list>
#include <stdlib.h>
#include <assert.h>
/* libs de: bluetooth, sistema, sockets  */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
/* libs propias de bluetooth */
#include <bt-manager/btsimpleserver.h>
#include <bt-manager/btdmanager.h>
#include <bt-manager/btdongledevice.h>
#include <bt-manager/btconnection.h>
#include <bt-manager/btconnmanager.h>
#include <bt-manager/btservermanager.h>
/* libs propias auxiliares */
#include "btprotocol.h"
#include "consts.h"
#include "debug.h"




/*! algunos defines que van a determinar que puerto usar en los servers */
#define BTRECEIVER_SERVER_PORT		11
/* cantidad de veces que vamos a intentar algo antes de determinarlo como 
 * error */
#define BTRECEIVER_MAX_TRIES		5


using namespace::std;


class BTReceiver {
	public:
		/* constructor */
		BTReceiver(/*!FIXME:UDataBase *udb*/);
		
		
		/* Funcion que genera los servidores, configura los dongles, y 
		 * comienza a escuchar. Seria la funcion de inicializacion.
		 * RETURNS:
		 *	< 0	on error
		 *	0	otherwise
		*/
		int startListen(void);
		
		/* Funcion que detiene todos los servidores.
		 * RETURNS:
		 *	0	if success
		 */
		int stopListen(void);
				
		/* Funcion que devuelve ante una nuva recepcion de datos, los
		 * datos recibidos por la conexion y la direccion de la misma.
		 * NOTE: Las nuevas conexiones/envio de datos se manejan 
		 * 	 automaticamente.
		 * REQUIRES:
		 * 	mac	!= NULL
		 * RETURNS:
		 * 	< 0 	on error
		 * 	0	if success
		 * 	data 	= data received
		 * 	mac	= remote mac address
		 */ 
		int getReceivedObject(string &data, bdaddr_t *mac);
		
		
		/* destructor */
		~BTReceiver(void);
		
	
	
	private:
		/* 	###		Funciones		###	*/
		
		/* Funcion que se encarga de inicializar un dongle device 
		 * REQUIRES:
		 * 	dongle 	!= NULL
		 * RETURNS:
		 * 	< 0	on error
		 *	0	if success
		*/
		int initializeDongle(BTDongleDevice *dongle);
		
		/* Funcion que genera e inicializa un server para un dongle
		 * determinado (hace el bind y el listen..)
		 * REQIURES:
		 * 	dongle 	!= NULL
		 * RETURNS:
		 * 	server 	!= NULL	on success
		 * 	NULL		on errorr
		 */
		BTSimpleServer *createServer(BTDongleDevice *dongle);
		
		/* Funcion que se encarga de determinar si una mac es admisible
		 * o no (se encuentra en la base de datos).
		 * RETURNS:
		 * 	true	if is in the db
		 * 	false	otherwise
		 */
		bool isMacInDB(const bdaddr_t &mac);
		
		
		/* Funcion que envia un string con todos los dongles del sistema
		 * formateados y respetando el protocolo listo para mandar
		 * a una conexion.
		 * REQUIRES:
		 * 	con 	!= NULL
		 */
		void sendDongleList(BTConnection *con);
		
		/* Funcion que dada una conexion, determina si la conexion debe
		 * ser cerrada, debe seguir recibiendo datos, o si ya tiene
		 * datos completos y los agrega a msg, ademas de que elimina
		 * del buffer de recepcion los datos.
		 * REQUIRES:
		 * 	con != NULL
		 * RETURNS:
		 * 	-1	si la conexion debe ser cerrada
		 * 	0	si tenemos datos completos.
		 * 	1	si tenemos datos impletos pero correctos.
		 * 	msg 	= mensaje extraido (si no hubo error).
		 */
		int checkConnection(BTConnection *con, string &msg);
		
		
		
		/*	###		Atributos		###	*/
		/* dongle manager */
		BTDManager *dManager;
		/* lista de donglesMac formateadas para ser enviadas a las con */
		string donglesMacs;
		/* Manejador de conexiones */
		BTConnManager connManager;
		/* Base de datos de usuarios */
		/*!FIXME UDataBase *udb;*/
		
		
		
	
	
};


#endif
