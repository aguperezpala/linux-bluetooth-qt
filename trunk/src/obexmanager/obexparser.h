/*! 		MODULO FUNCIONAL, no es un TAD
 * Simplemente se encarga de respetar el protocolo cuando recibe los datos.
 * ### NOTE: esta clase esta media fea con el nombre, ya que seria una especie
 * de ProtocolManager, que se encarga de generar usuarios a aprtir de requests.
 * No solo parsea, hace mucho mas que eso. (fea modularizacion)
*/
#ifndef USPARSER_H
#define USPARSER_H


#include <string.h>
#include <QString>

#include "../cuser/cuser.h"
#include "../consts.h"
#include "../debug.h"

#define USP_SSAP_MAZ_SIZE	256
#define USP_HEADER		"<SSAP>"
#define USP_REGISTER		"registrar"

class USParser{
	public:
		/* Constructor: */
		USParser ();
		
		/* Funcion que chequea si un request es valido correspondiente
		 * al protocolo SSAP.
		 * REQUIRES:
		 * 	req.isNull() == false
		 * RETURNS:
		 *	-1	si esta incompleto.
		 *	-2	si tiene algun error.
		 *	0	si no hubo error y es paquete
		 *		<SSAP>MAC,nick<SSAP>.
		 *	1	si no hubo error y es un <SSAP>registrar<SSAP>
		 */
		int isValidRequest(QString & req);
		
		
		/* Funcion que genera un user desde un request valido
		 * REQUIRES:
		 *	req.isNull() == false
		 *	isValidRequest(req) == 0
		 * RETURNS:
		 *	cuser != NULL 	if success
		 *	NULL		on error
		 */
		 /*! NOTE: FEA MODULARIZACION porque estamos chequeando el
		 * protocolo y los datos en la misma funcion, queda feo!
		 */
		CUser * parseRequest(QString & req);
		
		
		/*! Otra funcion irracional para el parser :p.*/
		/* Funcion que se encargar de convertir un response en el 
		 * protocolo determinado.
		 * REQUIRES:
		 *	resp.isNull() == false
		 * RETURNS:
		 *	true 	if success
		 *	false 	if error (resp.size >= USP_SSAP_MAZ_SIZE)
		 * NOTE: modifica response si y solo si no execede el tamaño
		 *       maximo.
		 */
		bool createResponse (QString & response);
		
		/* destructor que no hace nada :) */
		~USParser();
		 
	private:
		/* Funcion que va a determinar si un paquete valido es del tipo
		 * de "registracion" o del tipo de MAC,Nickname.
		 * REQUIRES:
		 *	req.isNull() == false
		 * RETURNS:
		 *	-1 	error
		 *	0	si es del tipo (MAC,nick)
		 *	1	si es del tipo "registrar"
		 */
		int getReqKind (QString req);
		
};

#endif