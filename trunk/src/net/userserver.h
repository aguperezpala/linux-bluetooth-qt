/*!vamos a implementar esta clase en C"
Se podria decir que es un modulo independiente. Ya que va a ser creado como un
thread diferente

REQUIERE de una userlist para agregar los usuarios.
*/
#ifndef USERSERVER_H
#define USERSERVER_H

#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <QString>	/*para facilitar las cosas :D*/


#include "../userspp/userlist.h"
#include "../debug.h"
/*rango en el que vamos a intentar establecer una coneccion*/
#define US_MINOR_PORT_RANGE	1234
#define US_MAYOR_PORT_RANGE	1239

#define US_MAX_BUFFER_SIZE	500

/*!aca vamos a definir los datos que querramos tener en cuenta (osea que vamos
 *a recibir)
el formato va a ser:
		$VARIABLE=DATO$\n
todo CON MAYUSCULA.
en este caso solamente vamos a recibir el numero de telefono (y el nickname)
		$NICK=Agu$\n
		$NUMBER=3516545682$\n
*/
#define US_VAR_NICK	"NICK"
#define US_VAR_NUMBER "NUMBER"


class UserServer {
	
public:
	/*le tenemos que pasar la userlist en la que vamos a escribir
	REQUIRES:
			UserList != NULL;
	*/
	UserServer(UserList*);
	
	/*esta funcion nos va a a poner a escuchar en algun puerto del rango
	especificado
		RETURNS:
				true == se pudo conectar
				false == cc
	*/
	bool startListen();
	/*
	RETURNS:
			true == se pudo conectar
			false == cc
	*/
	bool startListenOn(int port);
	
	void start();	/*esta funcion es un bucle quasi infinito*/
	
	
	
	/*funcion que determina si esta activo o no el servidor*/
	bool isActive();
	
	int getPort();
	
	void closeServer();
	
	~UserServer();
	
private:
	/*esta funcion lo que nos hace es setear el campo src, segun lo obtenido
	 *del cliente y el tipo de var que es
		REQUIRES:
				var != NULL
				src != NULL
	*/
	void parseVar (QString& src, const char *var);
	
	bool status;
	UserList *userlist;
	int sock;
	struct sockaddr_in self;
	char buffer[US_MAX_BUFFER_SIZE];
	
};
	





#endif
