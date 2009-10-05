/*! Practicamente es el modulo que se encarga de manejar las conexiones y ese
 * tipo de cosas.
 * Tambien en un futuro va a tener las funciones que se van a encargar de
 * ejecutar las RPC ("Remote Procedure Call") :).
*/
#ifndef UDBSERVER_H
#define UDBSERVER_H

#include <QString>
#include <stdio.h>

#include "udbprotocol.h"
#include "../../debug.h"
#include "../../consts.h"
#include "../../simple_server/sclient.h"
#include "../udatabase.h"
#include "../../simple_server/sserver.h"

/*! Vamos a definir el modelo de funcion que van a ser las RPC:
 * int (func*) (void* req, void* resp);
 * Donde:
 * valor devuelto: errorCode
 * req: datos nescesarios para ejecutar la funcion (generalmente QString).
 * resp: QString, generalmente donde vamos a modificar el QString y almacenar
 *	la respuesta de la funcion (sin parsear en el protocolo).
 * NOTE: Cada funcion chequea si esta bien formada la funcion o no.
 */
typedef int (func*)(void*,void*) rpcFunc_t;

/* Definimos un tipo usado solo aca, feo pero util :) */
typedef struct {
	const char* funcName;
	rpcFunc_t func;
} rpcTuple_t;


/* Definimos la lista de funciones RPC, el nombre indica claramente que lo que
 * deberia hacer la funcion. */

int udbs_RPCgetUserFromMac (void * req, void * resp);


/*! Ahora vamos a definir un arreglo de tuplas para determinar las funciones
 * y los punteros a las funciones correspondientes. El arreglo tiene que
 * si o si terminar en NULL, para saber en donde termina.
 */
rpcTuple_t RPC_FUNCS[2] = {
	{"getUserFromMac", udbs_RPCgetUserFromMac},
	NULL
};


/* Funcion que espera que un usuario se registre, chequeando el protocolo
 * y asegurandose que todo este "bajo control".
 * NOTE: es bloqueante (modificar para que no sea asi?).
 * REQUIRES:
 *	client != NULL
 * RETURNS:
 *	< 0 	on error	(No respeta el protocolo. NOTE: no cierra ni
 *				 libera el cliente, desde mas arriba hacerlo).
 *	== 0	if no error.
 * NOTE: limpia el buffer del cliente apenas se llama a esta funcion
 */
int udbs_registerClient (SClient * client);

/* Funcion que espera recibir una cadena sin errores y completa de un cliente
 * determinado, para que cuando se obtenga el request del cliente, sea un
 * request "correcto" en cuanto a protocolo.
 * NOTE: es bloqueante (modificar dividiendola en 2 partes a la funcion:
 * Ya que va a tener que devolver "incomplete_info" en caso de no haber recibido
 * todos los datos).
* REQUIRES:
*	client != NULL
* RETURNS:
*	< 0 	on error
*	0 	if NO error (NOTE: ENSURES: udbs_isValidRequest (client->data) 
*				  == UDBS_NO_ERROR)
* NOTE: RESPONDE con un "ok" en caso de no haber encontrado error, para respetar
* 	el protocolo.
*/
int udbs_receiveReq (SClient * client);

/*! funcion super compleja, Se encarga de trabajar las RPC, deberiamos hacer
 * una arreglo de tuplas (string, func pointer) para determinar cada una
 * de las RPC
 */
/* Funcion que va a ejecutar una RPC de un cliente, y a su vez responderle
 * con los datos nescesarios. Es una funcion compleja.
 * REQUIRES:
 *	client != NULL
 *	is_validRequest (client->data)
 *	udb 	!= NULL
 * RETURNS:
 *	-1	si hubo un pedido fallido (no respeta el protocolo. NOTE: NO se
 *		cierra la conexion, ni se libera el client, hacerlo de arriba).
 *	-2	si hubo un error interno...
 *	== 0 	NO hubo error
 */
int udbs_RPCWork (SClient * client, const UDataBase * udb);


#endif