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
#include <bt-manager/btsdpsessiondata.h>
/* libs propias auxiliares */
#include "udatabase.h"
#include "cuser.h"
#include "btprotocol.h"
#include "btpaket.h"
/* code generator */
#include "btcodeadmin.h"
/* const / debug */
#include "consts.h"
#include "debug.h"




/*! algunos defines que van a determinar que puerto usar en los servers */
#define BTRECEIVER_SERVER_PORT		11
/* cantidad de veces que vamos a intentar algo antes de determinarlo como 
 * error */
#define BTRECEIVER_MAX_TRIES		5
/*! FIXME vamos a definir estaticamente el UUID, cambiar esto... */
#define BTRECEIVER_UUID		0 , 0 , 0 , 0x0003 
/*! Vamos a determinar que nombre de archivo vamos a guarcar los codigos */
#define BTRECEIVER_CODES_FILE	"code_file.codes"

using namespace::std;


class BTReceiver {
	public:
		/* constructor 
		 * REQUIRES:
		 * 	udb != NULL
		 * 	load = (true => load BTCodeAdmin from file, false = new)
		*/
		BTReceiver(UDataBase *udb, bool load = true);
		
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
		 * 	pkt 	= paket received
		 * 	mac	= remote mac address
		 */ 
		int getReceivedObject(BTPaket &pkt, bdaddr_t *mac);
		
		/* Funcion que devuelve el dongle manager utilizado */
		BTDManager *getDongleManager(void){return this->dManager;};
		
		/* Funcion que devuelve la lista de conexiones con la que esta
		 * trabajando:
		 * NOTE: TENER CUIDADO con manipular conexiones cuando se esta
		 * 	 llamando a getReceivedObject desde otro thread.
		 * NOTE 2: NO borrar ninguna conexion.
		 */
		const list<BTConnection *>& getConList(void);
		
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
		
		/* Funcion que setea una SDP asociada a un server. 
		 * REQUIRES:
		 * 	UUID	!= NULL
		 * 	dongle 	!= NULL
		 * 	port
		 * RETURNS:
		 * 	0	if success
		 * 	< 0	on error
		 */
		int createSdpSession(uint32_t *uuid, BTDongleDevice *dongle, 
				      int port);
		
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
		
		/* Funcion que registra un nuevo usuario en la base de datos
		 * REQUIRES:
		 * 	pkt
		 * RETURNS:
		 * 	< 0	on error
		 * 	0	if succes
		 */
		int registerNewUser(const bdaddr_t *mac, BTPaket &pkt);
		
		/* Funcion que dada una conexion, determina si la conexion debe
		 * ser cerrada, debe seguir recibiendo datos, o si ya tiene
		 * datos completos y rellena el paket, ademas de que elimina
		 * del buffer de recepcion los datos.
		 * REQUIRES:
		 * 	con != NULL
		 * RETURNS:
		 * 	-1	si la conexion debe ser cerrada
		 * 	0	si tenemos datos completos.
		 * 	1	si tenemos datos impletos pero correctos.
		 * 	pkt 	paquete extraido (si no hubo error).
		 */
		int checkConnection(BTConnection *con, BTPaket &pkt);
		
		
		/*	###		Atributos		###	*/
		/* dongle manager */
		BTDManager *dManager;
		/* lista de donglesMac formateadas para ser enviadas a las con */
		string donglesMacs;
		/* Manejador de conexiones */
		BTConnManager connManager;
		/* Base de datos de usuarios */
		UDataBase *udb;
		/* administrador de codigos */
		BTCodeAdmin codAdmin;
		
		
		
	
	
};


#endif
