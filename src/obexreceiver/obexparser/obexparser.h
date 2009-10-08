/*! Modulo que va a parsear una cadena de caracteres y a su vez va a  
 * modificar el QByteArray
 * 
*/
#ifndef OBEXPARSER_H
#define OBEXPARSER_H


#include <QByteArray>
#include <QStringList>
#include <QString>
#include <string.h>
#include "../../consts.h"
#include "../../debug.h"


/* Funcion que parsea segun el pseudo_protocolo los datos del buffer.
 * Esta funcion se fija simplemente en el protocolo y devuelve una
 * QStringList, donde cada elemento es uno de los de la tupla.
 * En caso de encontrar incosistencias elimina automaticamente las cosas
 * del buffer.
 * REQUIRES:
 *	buff.isNull() == false && buff.size() > 0
 * RETURNS:
 * 	NULL	sii (No hay paquete completo)
 *	strList != NULL en caso de encontrar una tupla.
 * NOTE: En caso de incosistencias => eliminamos los datos hasta encontrar
 *	datos consistentes.
 * NOTE 2: En caso de que haya mas de un paquete, devolvemos solo el primero y
 *	   los otros no son modificados (volver a llamar a esta funcion).
 * NOTE 3: el parser NO sabe si los datos recibidos son correctos.
 */
 QStringList * obpa_parse (QByteArray & buff);



#endif