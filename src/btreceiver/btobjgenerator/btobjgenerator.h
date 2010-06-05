#ifndef BTOBJGENERATOR_H
#define BTOBJGENERATOR_H


#include <QString>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <bluetooth/bluetooth.h>
/* libs propias */
#include "dispobject.h"
#include "cuser.h"
#include "udatabase.h"
/* de bt */
#include "btreceiver.h"
#include "btpaket.h"
#include "consts.h"
#include "debug.h"




using namespace::std;


class BTObjGenerator{
	public:
		/* constructor, necesita de un BTReceiver...
		 * REQUIRES
		 * 	receiver 	!= NULL
		 * 	udb 		!= NULL
		 * NOTE: inicializa el btreceiver.
		 */
		BTObjGenerator(BTReceiver *receiver, UDataBase *udb);
		
		/*! Funcion bloqueante que lo que hace es simplemente generar
		 * DispObjects desde las cosas recibidas por medio del 
		 * BTReceiver..
		 * REQUIRES:
		 * 	errCode (< 0 => error; 0 => success
		 * RETURNS:
		 * 	NULL	on error
		 * 	dObj	otherwise
		 * NOTE: Genera memoria
		 */
		DispObject *getDispObject(int &errCode);
		
		
		/* destructor.
		 * NOTE: no libera el btReceiver
		 */
		~BTObjGenerator(void);
		
	
	
	private:
		/*	###		Funciones 		###	*/
		
		/* Funcion que genera un objeto si lo recibido es un archivo
		 * REQUIRES:
		 * 	pkt.cmdType == BT_CMD_FILE
		 * RETURNS:
		 * 	NULL	if error
		 * 	dobj	if success
		 */
		DispObject *dObjFromFile(BTPaket &pkt);
		
		/* Funcion que genera un objeto si lo recibido es un texto
		* REQUIRES:
		* 	pkt.cmdType == BT_CMD_TEXT
		* RETURNS:
		* 	NULL	if error
		* 	dobj	if success
		*/
		DispObject *dObjFromText(BTPaket &pkt);
		
		
		
		/*	###		Atributos		###	*/
		/* receiver */
		BTReceiver *btReceiver;
		UDataBase *udb;
		
		
		
		
	
	
};


#endif
