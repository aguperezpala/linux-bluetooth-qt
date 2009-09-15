#include "usparser.h"

/* Constructor: */
USParser::USParser () 
{
	/* do nothing XD*/
	return;
}



/* Funcion que va a determinar si un paquete valido es del tipo
* de "registracion" o del tipo de MAC,Nickname.
* REQUIRES:
*	req.isNull() == false
* RETURNS:
*	-1 	error
*	0	si es del tipo (MAC,nick)
*	1	si es del tipo "registrar"
*/
int USParser::getReqKind (QString req)
{
	int respCode = 0;
	
	ASSERT (req.isNull() == false);
	
	if (req == USP_HEADER USP_REGISTER USP_HEADER)
		respCode = 1;
	else if (req[MAC_ADDR_SIZE + strlen (USP_HEADER)] == ',') 
		/* si tiene la coma bien puesta => listo */
		respCode = 0;
	else
		respCode = -1;
	
	return respCode;
}


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
int USParser::isValidRequest(QString & req)
{
	int respCode = 0;
	QString header = USP_HEADER;
	
	/* PRES */
	ASSERT (req.isNull() == false);
	/* chequeamos el tamaño */
	if (req.length() >= USP_SSAP_MAZ_SIZE) {
		debugp ("USParser::isValidRequest: req mayor de 256\n");
		return -2;
	}
	
	/* verificamos que si hay datos suficientes como para contener el
	* USP_HEADER entonces tiene que ser eso. */
	if (req.length () < header.length()) {
		/* no hay suficientes datos => incompleto entonces */
		debugp ("USParser::isValidRequest: incompleto, no tenemos "
		"el primer header todavia\n");
		return -1;
	}
	
	/* si hay mas caracteres o igual entonces => verificamos que son los 
	 * del header */
	if (req.indexOf(header) < 0) { /* no encontro el header => error */
		debugp ("USParser::isValidRequest: NO existe el primer "
		"header\n");
		return -2;
	}
	
	/*! si estamos aca es porque tenemos al principio el header */
	/* verificamos que tengamos el header final, si no es incompleto */
	if (req.indexOf(header,2) < 0) { /* no encontro el 2 header */
		debugp ("USParser::isValidRequest: NO esta el 2º header\n");
		return -1;
	}
	
	/* ultimo chequeo, eso es error, estan intentando mandar 2 pedidos
	 * simultaneos... ? */
	if (req.indexOf (header,2) + header.length() != req.length()) {
		debugp ("USParser::isValidRequest: Nos estan mandando de "
		"mas???\n");
		return -2;
	}
	
	/*! si estamos aca es porque en teoria tenemos bien formado en cuanto
	 * a protocolo, ahora verificamos si llegaron los datos */
	/* verificamos que tipo de request es */
	respCode = getReqKind(req);
	if (respCode < 0) { /* cambiamos el tipo de error */
		debugp ("USParser::isValidRequest: en getReqKind ()\n");
		respCode = -2;
	}
	
	return respCode;
}
	


/* Funcion que genera un user desde un request valido
* REQUIRES:
*	req.isNull() == false
*	isValidRequest(req) == 0
* RETURNS:
*	cuser != NULL 	if success
*	NULL		on error
*/
CUser * USParser::parseRequest(QString & req)
{
	CUser * user = NULL;
	QString cleanReq = req;
	QString header = USP_HEADER;
	QString MAC = "", nick = "";
	
	/* pres */
	ASSERT (req.isNull() == false);
	ASSERT (isValidRequest(req) == 0);
	
	/* limpiamos un poco el request */
	cleanReq.remove (header);
	
	/* extraemos las cosas */
	MAC = cleanReq.mid (0, MAC_ADDR_SIZE);
	nick = cleanReq.mid (MAC_ADDR_SIZE + 1, 
			      cleanReq.length() - (MAC_ADDR_SIZE + 1));
	
	user = new CUser (&nick, &MAC);
	
	
	
	return user;
}
	


/*! Otra funcion irracional para el parser :p.*/
/* Funcion que se encargar de convertir un response en el 
* protocolo determinado.
* REQUIRES:
*	resp.isNull() == false
* RETURNS:
*	true 	if success
*	false 	if error (resp.size > USP_SSAP_MAZ_SIZE)
* NOTE: modifica response.
*/
bool USParser::createResponse (QString & response)
{
	QString header = USP_HEADER;
	
	/* pre */
	ASSERT (response.isNull() == false);
	
	/* simplemente agregamos los headers y verificamos si se puede o no */
	if (header.length() * 2 + response.length() > USP_SSAP_MAZ_SIZE)
		return false;
	
	/*! si estamos aca es porque si podemos agregar */
	response.prepend (header);
	response.append (header);
	
	return true;
	


/* destructor que no hace nada :) */
USParser::~USParser()
{
	/* do nothing */
	return;
}

