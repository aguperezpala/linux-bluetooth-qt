#include "obexparser.h"



/* Esta funcion lo que hace es devolver un QStringList (deberiamos tener un 
 * paquete).
 * REQUIRES:
 * 	buff.isNull () == false
 * 	existPackage (buff)
 * RETURNS:
 * 	NULL 		on error
 * 	strList		on success
 */
static QStringList * obpa_getList (QByteArray & buff)
{
	int startPos = 0, endPos = 0;
	QStringList * result = NULL;
	QString data = "";
	
	/* pres */
	if (buff.isNull() || buf.size() == 0) {
		ASSERT (false);
		return NULL;
	}
	startPos = buff.indexOf (OBREC_PKT_SEPARATOR_B, 0);
	endPos = buff.indexOf (OBREC_PKT_SEPARATOR_E, startPos + 1);
	
	/*! Pre 2 */
	if ((startPos < 0) || (endPos <= 0) {
		ASSERT (false);
		return NULL;
	}
	
	/*! tamos joia => buscamos primero el file y luego la MAC */
	
	

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
 */
 QStringList * obpa_parse (QByteArray & buff)
{
	QStringList * result = NULL;
	int startPos = 0, endPos = 0;
	
	
	/* Pres */
	if (buf.isNull() || buf.size() <= 0) {
		ASSERT (false);
		return result;
	}
	
	/* primero que todo vamos a buscar el comienzo del pakete */
	startPos = buff.indexOf(OBREC_PKT_SEPARATOR_B, 0);
	/* verificamos si hay basura en los primeros bytes */
	if (startPos > 0)
		buff.remove (0, startPos);
	else if ((startPos < 0) && (buff.size() > OBREC_MAX_PKT_SIZE)) {
		/* nos tamos iendo al chori => limpiamos */
		buff.clear();
		return result;
	}
	
	/* calculamos el OBREC_PKT_SEPARATOR final */
	endPos = buff.indexOf (OBREC_PKT_SEPARATOR_E, 1);
	
	/* ahora verificamos que exista tanto el indicado final como el inical
	 * y ademas el SIZE no se haya exedido => estan escribiendo verduras?
	*/
	/*! primero verificamos lo mas importante para que no haya overflow */
	if ((endPos <= 0) && (buff.size() > OBREC_MAX_PKT_SIZE)){
		/* liberamos todo el buffer, nunca hubo un final */
		buff.clear();
		return result;
	} else if (endPos > OBREC_MAX_PKT_SIZE) {
		/* tambien lo debemos liberar, pero hasta endPos */
		buff.remove(0, endPos + 1);
		return result;
	} else if (startPos < 0 || endPos <= 0) 
		/* estamos aca entonces quiere decir que no tenemos nada => 
		 * devolvemos NULL */
		return result;
	
	/* si estamos aca es porque en teoria tenemos un paquete... */
	result = obpa_getList (buff);
	
	return result;
}