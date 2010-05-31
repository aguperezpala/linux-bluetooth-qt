#ifndef BTPAKET_H
#define BTPAKET_H

#include <string>
#include <stdio.h>

#include "btprotocol.h"


/* determinamos si el pakete fue recibido, o esta para ser enviado */
enum {
	BT_PKT_RCV,
	BT_PKT_SND
};



using namespace::std;


class BTPaket {
	public:
		/* constructor */
		BTPaket(void);
		
		/* Funcion que llena un pakete segun el protocolo determinado
		 * RETURNS:
		 * 	< 0	if error (no respeta el protocolo)
		 *	0	on success
		 */
		int pktFromRcvString(string &data);
		
		/* Funcion que devuelve el tipo de pakete (envio o recepcion) */
		int getOrigination(void){return this->origination;};
		
		/* Funcion que devuelve el tipo de comando.
		 * RETURNS:
		 * 	-1		if the command its unknown
		 *	cmtType		otherwise
		 */
		int getCmdType(void){return this->cmdType;};
		
		/* Funcion que devuelve una referencia del mensaje 
		 * NOTE: cuidado con cambiar los datos
		 */
		string &getMsg(void){return this->msg;};
		
		/* Funcion que setea el origen del paquete 
		 * {BT_PKT_RCV | BT_PKT_SND}
		 */
		void setOrigination(int o);
		
		/* Funcion que setea el tipo de commando commando */
		void setCmdType(int ct);
		
		/* Funcion que setea los datos del pakete */
		void setMsg(string &msg);
		
		/* funcion que llena los datos segun un commando y un mensaje
		 * RETURNS:
		 * 	< 0	on error
		 * 	== 0	if success
		 */
		int pktFromCmdAndMsg(string &cmd, string &msg);
		
		/* Funcion que convierte el pakete en una cadena de caracteres
		 * listo para enviarse.
		 * RETURNS:
		 * 	NULL	if error
		 * 	strPkt	on success
		 * NOTE: Genera memoria
		 */
		string *toString(void);
		
		/* destructor */
		~BTPaket(void);
		
	
	
	private:
		/*	Atributos	*/
		int origination;	/* fue recibido o para enviar? */
		int cmdType;
		string msg;
	
	
};

#endif
