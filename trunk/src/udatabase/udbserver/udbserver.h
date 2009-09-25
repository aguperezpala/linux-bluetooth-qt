/*! Esta "base de datos" va a estar implementada con una hash de usuarios, que
 * lo que va hacer es almacenar usuarios en el disco duro y en la hash.
 * La forma en que se van almacenar los archivos en el disco va a ser:
 * MAC,Nickname\n
 * Uno por fila, dividiendo la MAC del Nickname por el caracter ','
*/
#ifndef UDBSERVER_H
#define UDBSERVER_H


/* librerias generales */
#include <stdio.h>	
#include <stdlib.h>
#include <string.h>
/* sockets */
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>

#include <QString>

#include "../../cuser/cuser.h"
#include "../../consts.h"
#include "../../debug.h"
#include "../udatabase.h"


class UDBServer {
	public:
		/* Constructor: no requiere nada :)
		*/
		UDBServer (void);
		
		
		
		/* Destructor */
		~UDBServer();
		
	private:
		
};

#endif