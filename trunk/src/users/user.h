/* Las partes del "objeto" user las vamos a manejar todas con char* para facilitar
 * el trabajo de "matcheo"
 */
#ifndef USER_C_H
#define USER_C_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*	Librerias personales	*/
#include "../debug.h"

/* Practicamente estos defines no los vamos a tocar nunca*/
#define USR_MAX_DNI_SIZE	9
#define USR_MAX_NAME_SIZE	30
#define USR_MAX_NICK_SIZE	10	/*el nombre que se va a mostrar en pantalla*/
#define USR_MAX_NUM_SIZE	15	/*supongo que no hay mas que eso*/
#define USR_MAX_DATE_SIZE	20	/*registrar la fecha que se inscribio..*/


#define EMPTY	NULL	/*por conveniencia*/

typedef struct _user_t user_t;

/*solo vamos a pedir los datos mas importantes
	REQUIRES:
		nada en especial, 
  si nick == EMPTY seteamos 1º nombre del name (si name != EMPTY)
  si algun otro param == EMPTY entonces directamente no se agrega ese campo
  size(param)> USR_MAX_$param$ entonces se trunca hasta USR_MAX_$param$

	RETURNS:
		result == EMPTY ==> error (algo raro)
		result != EMPTY ==> ok
*/
user_t *user_new (char *name, char *nick, char *num);

/*! Para estas funciones vamos a tener en cuenta que si estamos intentando
 * setear algun tipo de dato a la estructura "self" y self == EMPTY entonces
 * vamos a devolver false
*/


bool user_set_name (user_t *self, char *name);	/*usamos la politica de arriva*/

bool user_set_nick (user_t *self, char *nick);	/*usamos la politica de arriva*/

bool user_set_number (user_t *self, char *num);	/*usamos la politica de arriva*/

bool user_set_dni (user_t *self, char *dni);	/*usamos la politica de arriva*/

bool user_set_date (user_t *self, char *date);	/*usamos la politica de arriva*/


/*!Para get vamos a implementar la politica de que si pedimos algun campo, puede
 * suceder que no este definido, por lo que vamos a devolver  EMPTY si es que no
 * existe, en caso contrario vamos a devolver el puntero al campo.
 * En caso de self == NULL return NULL
 */ 


const char* user_get_name (user_t *self);	/*usamos la politica de arriva*/

const char* user_get_nick (user_t *self);	/*usamos la politica de arriva*/

const char* user_get_number (user_t *self);	/*usamos la politica de arriva*/

const char* user_get_dni (user_t *self);	/*usamos la politica de arriva*/

const char* user_get_date (user_t *self);	/*usamos la politica de arriva*/


/*	REQUIRES:
		Nada.. si self == EMPTY ==> RETURN EMPTY
*/
user_t *user_destroy (user_t *self);


#endif
