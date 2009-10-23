/*! Este tad nos va a proporcionar la conexion con la base de da datos de
 * nuestro sistema. Para agregar usuarios y consultar si existen.
 * Este TAD ademas soporta la multiple concurrencia.
 */
#ifndef OOBEXDBC_H
#define OOBEXDBC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "client.h"

typedef struct _oDBCon oDBCon_t;

/* Constructor */
oDBCon_t * odbc_create (void);

/* destructor.
 * REQUIRES:
 * 	odbc != NULL
 */
void odbc_destroy (oDBCon_t * obdc);

/* Funcion que establece la conexion con la base de datos, y se registra.
 * REQUIRES:
 *	odbc 	!= NULL
 *	ip	!= NULL
 *	port	> 0
 * RETURNS:
 *	0	if success
 *	errCode on error
*/
int odbc_connect (oDBCon_t * obdc, const char * ip, short port);

/* Funcion que sirve para desconectarse de la base de datos.
 * REQUIRES:
 *	odbc 	!= NULL
 */
void odbc_disconnect (oDBCon_t * obdc);

/* Funcion que chequea si estamos conectados o no a la base de datos
 * REQUIRES:
 *	odbc != NULL
 * RETURNS:
 *	true 	if we are connected
 *	false	otherwise
 */
bool odbc_is_connected (oDBCon_t * obdc);

/* Se busca en la base de datos la dirección MAC pasada como parámetro
 * Si la misma está registrada. De ser así se retorna el nombre de usuario
 * asociado. Sino se devuelve NULL.
 *
 * PRE: MAC 	!= NULL
 *	odbc 	!= NULL
 *	odbc_is_connected(odbc);
 *
 * CALL: usr = existUser (MAC)
 *
 * POS: usr != NULL && "El nombre de usuario asociado a MAC es 'usr'"
 *	  OR
 *	usr == NULL &&  ("No hay conexión con la BD"
 *			   OR
 *			 "El cliente no está registrado en la BD")
 *//*! NOTE: ATOMIC */
char *odbc_existUser (oDBCon_t * odbc, const char *MAC);

/* Funcion que va a agregar un usuario a la base de datos.
 * REQUIRES:
 *	MAC 	!= NULL
 *	nick	!= NULL
 *	odbc 	!= NULL
 *	odbc_is_connected(odbc);
 * RETURNS:
 *	true	on success
 *	false	otherwise
 *//*! NOTE: ATOMIC */
bool odbc_addUser (oDBCon_t * odbc, const char * MAC, const char * nick);
 

#endif
