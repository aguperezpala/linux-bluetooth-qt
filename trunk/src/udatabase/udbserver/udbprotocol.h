#ifndef UDB_PROTOCOL_H
#define UDB_PROTOCOL_H


#include <QString>

#include "../../debug.h"
#include "../../consts.h"

/* definimos los rangos en los que va a escuchar el servidor de la base
* de datos. */
#define UDBSERVER_START_PORT		2345
#define UDBSERVER_END_PORT		2350


#define SSDBP_MAX_BUFF_SIZE		255
#define SSDBP_HEADER			"<SSDBP>"
/* Definimos que tipos posibles de codigos pueden ser devueltos por el
 * modulo de protocolo (este).
 */
typedef enum {
	UDBS_NO_ERROR,		/* no hay ningun error */
	UDBS_OVERFLOW,		/* recibimos mas de lo permitido */
	UDBS_NO_PROTOCOL,	/* no respeta el protocolo */
	UDBS_INCOMPLETE,	/* no tenemos un pedido completo posiblemente */
	UDBS_UNK_ERROR		/* error desconocido o interno */
} udbsCode_t;

/* Funcion que chequea si un request es correcto o no:
 * REQUIRES:
 *	req.isNull() == false
 * RETURNS:
 *	udbsCode_t
 */
udbsCode_t udbs_isValidRequest (const QString & req);

/* Funcion que toma una cadena de caracteres y la amolda al protocolo
 * para ser enviada.
 * REQUIRES:
 *	resp.isNull() == false
 * NOTE: el response (resp) mismo es modificado.
 */
void udbs_generateResponse (QString & resp);


/* Funcion que parsea los datos en formato del protocolo y los devuelve
 * "limpiamente" (sacandole los encabezados y demas.
 * REQUIRES:
 *	data.isNull() == false
 *	udbs_isValidRequest(data) == UDBS_NO_ERROR
 * RETURNS:
 *	qstr	!= NULL
 * 	NULL	otherwise
 * NOTE: GENERA MEMORIA => liberarla despues.
 */
QString * udbs_parseRequest (const QString & data);

#endif
