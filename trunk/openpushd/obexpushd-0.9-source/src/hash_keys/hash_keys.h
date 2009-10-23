/*! Este tad nos va a proporcionar la base de datos de claves disponibles
 * que van a poder registrar el sistema.
 * Va a soportar concurrencia.
 * NOTE: El archivo de claves debe tener una clave por linea y terminar en un \n
 */
#ifndef HASH_KEYS_H
#define HASH_KEYS_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <glib.h>
#include <pthread.h>

#include "../debug.h"




typedef struct _hashKeys hashKeys_t;


/* Constructor */
hashKeys_t * hash_k_create (void);

/* Destructor
 * REQUIRES:
 *	hk != NULL
 */
void hash_k_destroy (hashKeys_t * hk);

/* Funcion que va a cargar la hashKey desde un archivo.
 * REQUIRES:
 *	hk != NULL
 *	fname != NULL
 * NOTE: si ya existia la clave la reemplaza.
 */
void hash_k_load_from_file (hashKeys_t * hk, const char * fname);

/* Funcion que va a guardar las claves que quedan en un archivo. Si el archivo
*  ya existia este es borrado y creado de nuevo.
* REQUIRES:
*	hk != NULL
*	fname != NULL
*/
void hash_k_save_to_file (hashKeys_t * hk, const char * fname);

/* Funcion que verifica si existe una clave determinada.
* REQUIRES:
*	hk != NULL
*	key != NULL
* RETURNS:
*	true	if exists
*	false	otherwise
*/
bool hash_k_exist (hashKeys_t * hk, const char * key);


/* Funcion que elimina una clave determinada, si no existe no hace nada.
* REQUIRES:
*	hk != NULL
*	key != NULL
*/
void hash_k_remove (hashKeys_t * hk, const char * key);


/*! funcion que agrega una determinada clave, en este caso no la vamos a usar,
 * asique no la voy a implementar :p
 */



#endif
