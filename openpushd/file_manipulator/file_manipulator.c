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
		return false;
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
void fm_delete_file (const char* fname)
{
	if (!fname) {
		ASSERT(false);
		return;
	}
	/* borramos el archivo.. al pedo esta funcion.. */
	remove (fname);
}


/* Funcion que copia un archivo de un lugar determinado.
* REQUIRES:
*	fsrc 	!= NULL
*	fdest 	!= NULL
* RETURNS:
*	false	on error || !file_exists(fsrc)
*	true	on success
* NOTE: no hace falta que fsrc y fdest sean absolutos.
*/
bool fm_copy_file (const char * fsrc, const char * fdest)
{
	FILE *d = NULL, *s = NULL;
	char buffer[1024];
	size_t incount = 0;
	
	
	if (!fsrc || !fdest) {
		ASSERT(false);
		return false;
	}
	
	s = fopen(fsrc, "rb");
	if(s == NULL)
		return false;
	
	d = fopen(fdest, "wb");
	if(d == NULL)
	{
		fclose(s);
		return false;
	}
	
	incount = fread(buffer, sizeof(char), 1024, s);
	
	while(!feof(s))
	{	
		fwrite(buffer, sizeof(char), incount, d);
		incount = fread(buffer, sizeof(char), 1024, s);
	}
	
	
	fwrite(buffer, sizeof(char), incount, d);
	
	fclose(s);
	fclose(d);
	
	return true;
}

