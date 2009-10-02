#include "udbprotocol.h"

/* Funcion que chequea si un request es correcto o no:
* REQUIRES:
*	req.isNull() == false
* RETURNS:
*	udbsCode_t
*/
udbsCode_t udbs_isValidRequest (const QString & req)
{	
	int pos = 0;
	QString header = SSDBP_HEADER;
	
	/* PRES */
	ASSERT (req.isNull() == false);
	/* chequeamos el tamaño */
	if (req.length() >= SSDBP_MAX_BUFF_SIZE) {
		debugp ("SSDBP::isValidRequest: req mayor de 256\n");
		return UDBS_OVERFLOW;
	}
	
	/* verificamos que si hay datos suficientes como para contener el
	* USP_HEADER entonces tiene que ser eso. */
	if (req.length () < header.length()) {
		/* no hay suficientes datos => incompleto entonces */
		debugp ("SSDBP::isValidRequest: incompleto, no tenemos "
		"el primer header todavia\n");
		return UDBS_INCOMPLETE;
	}
	
	/* si hay mas caracteres o igual entonces => verificamos que son los 
	* del header */
	pos = req.indexOf(header);
	if (pos < 0 || pos > 0) { 
		/* no encontro el header o NO esta al comienzo => error */
		debugp ("SSDBP::isValidRequest: NO existe el primer "
		"header\n");
		return UDBS_NO_PROTOCOL;
	}
	
	/*! si estamos aca es porque tenemos al principio el header */
	/* verificamos que tengamos el header final, si no es incompleto */
	if (req.indexOf(header,2) < 0) { /* no encontro el 2 header */
		debugp ("SSDBP::isValidRequest: NO esta el 2º header\n");
		return UDBS_INCOMPLETE;
	}
	
	/* ultimo chequeo, eso es error, estan intentando mandar 2 pedidos
	* simultaneos... ? */
	if (req.indexOf (header,2) + header.length() != req.length()) {
		debugp ("SSDBP::isValidRequest: Nos estan mandando de "
		"mas???\n");
		return UDBS_NO_PROTOCOL;
	}
	
	return UDBS_NO_ERROR;
}

/* Funcion que toma una cadena de caracteres y la amolda al protocolo
* para ser enviada.
* REQUIRES:
*	resp.isNull() == false
* NOTE: el response (resp) mismo es modificado.
*/
void udbs_generateResponse (QString & resp)
{
	QString header = SSDBP_HEADER;
	
	/* Pre */
	ASSERT (resp.isNull() == false);
	if (resp.isNull())
		return;
	
	resp.prepend (header);
	resp.append (header);
}	


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
QString * udbs_parseRequest (const QString & data)
{
	QString * result = NULL;
	QString header = SSDBP_HEADER;
	
	/* pres */
	if (data.isNull() || udbs_isValidRequest(data) != UDBS_NO_ERROR) {
		ASSERT (false); /* debug por ahora :) */
		return result;	/* devolvemos null de pecho */
	}
	
	/* ahora lo que vamos hacer es buscar los encabezados y eliminarlos */
	result = new QString (data);
	ASSERT (result != NULL);
	/* limpiamos los headers */
	result->remove(header);
	
	
	return result;
}
	
