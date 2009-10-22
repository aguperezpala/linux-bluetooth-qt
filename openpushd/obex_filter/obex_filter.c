#include "obex_filter.h"

/* Funcion principal, que nos va a decir si un archivo va a ser aceptado SIEMPRE
* o no, dependiendo del tamaño y del nombre del archivo (extension).
* NOTE: Por el momento vamos a aceptar todo archivo que sea de texto/vnote o
*	 tenga tamaño menor que MAX_ACCEPTED_FILE_SIZE
* REQUIRES:
*	fname != NULL
*	fsize != NULL
* RETURNS:
*	true	if its always accepted
*	false 	otherwise
*/
bool obf_always_accpted_file (const char * fname, const char * fsize)
{
	int size = 0;
	char * pointPos = NULL;
	
	/* pres */
	if (!fname || !fsize) {
		ASSERT(false);
		return false;
	}
	
	/* obtenemos el tamaño */
	if (EOF == sscanf (fsize, "%d", &size))
		/* error */
		return false;
	
	/* no hubo error => chequeamos el tamaño y esas cosas */
	if (size > MAX_ACCEPTED_FILE_SIZE)
		return false;
	
	/* ahora vamos a verificar si estamos recibiendo un formato aceptable */
	pointPos = strchr(fname, '.');
	/* si no tiene punto => lo aceptamos */
	if (!pointPos)
		return true;
	/* si lo encontramos entonces verificamos que sea algun formato 
	 * aceptable */
	if (strstr(ACCEPTED_FILE_FORMATS, pointPos) == NULL)
		/* no encontramos un chori, chau */
		return false;
	
	return true;
}
