#include "file_manipulator.h"




/* Funcion que chequea si un archivo determinado existe o no.
* REQUIRES:
*	fname != NULL
* RETURNS:
*	true	if exists
*	false 	otherwise
*/
bool fm_file_exists (const char * fname)
{
	struct stat dummy;
	int result = 0;
	/* pre */
	if (!fname) {
		ASSERT (false);
		return;
	}
	result = stat (fname, &dummy);
	/* File found */
	if ( result == 0 )
	{
		return true;
	}
	return false;
}
	
	


/* Funcion que devuelve el path absoluto del archivo que se encuentra en el 
* directorio donde se esta corriendo el programa.
* Si se le pasa un fname NULL entonces lo que hace es devolver el directorio
* actual (pwd).
* RETURNS:
*	pwd ++ fname	if no error
*	NULL		otherwise
* NOTE: Genera memoria, debe ser liberada despues.
*/
char * fm_get_absolute_path (const char * fname)
{
	char * result = NULL;
	
	result = get_current_dir_name();
	
	/* es null? => chau */
	if (!result)
		return result;
	
	/* nos dieron algun nombre de archivo */
	if (fname) {
		size_t newSize = 0;
		/* le vamos a concatenar el fname */
		newSize = strlen(result) + 3 /* '/' */ + strlen (fname);
		result = realloc (result, newSize);
		/* ahora le agregamos las cosas */
		strcat (result, "/");
		strcat (result, fname);
	}
	return result;
}



/* Funcion que se encarga de borrar un archivo.
* REQUIRES:
*	fname != NULL
*	file_exists (fname)
* POST:
*	fname is erased
*/
void fm_delete_file (const char* fname);


/* Funcion que copia un archivo de un lugar determinado.
* REQUIRES:
*	fsrc 	!= NULL
*	fdest 	!= NULL
* RETURNS:
*	false	on error || !file_exists(fsrc)
*	true	on success
* NOTE: no hace falta que fsrc y fdest sean absolutos.
*/
bool fm_move_file (const char * fsrc, const char * fdest);

