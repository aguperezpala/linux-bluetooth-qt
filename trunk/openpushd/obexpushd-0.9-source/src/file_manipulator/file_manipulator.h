/*! Con este modulo vamos a manejar el tema de los archivos, copiado/creacion/
 * existencia/etc.
 */
#ifndef FILE_MANIPULATOR_H
#define FILE_MANIPULATOR_H

#define _GNU_SOURCE	/* para el get_current_directory */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>	/* file_exists */
#include <stdbool.h>
#include <unistd.h>	/* get_current_directory() */
#include <string.h>

#include "../debug.h"


/* Funcion que chequea si un archivo determinado existe o no.
 * REQUIRES:
 *	fname != NULL
 * RETURNS:
 *	true	if exists
 *	false 	otherwise
 */
bool fm_file_exists (const char * fname);


/* Funcion que devuelve el path absoluto del archivo que se encuentra en el 
 * directorio donde se esta corriendo el programa.
 * Si se le pasa un fname NULL entonces lo que hace es devolver el directorio
 * actual (pwd).
 * RETURNS:
 *	pwd ++ fname	if no error
 *	NULL		otherwise
 * NOTE: Genera memoria, debe ser liberada despues.
 */
char * fm_get_absolute_path (const char * fname);


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
bool fm_copy_file (const char * fsrc, const char * fdest);


#endif
