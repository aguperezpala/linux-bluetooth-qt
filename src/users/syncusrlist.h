/* Este modulo lo vamos a usar para syncronizar el acceso a la lista de usuarios.
 * Vamos a tener en cuenta que el dato mas importante de un usuario es el numero
 * de telefono.
 *
 * Por ahora es ineficiente esta forma, despues cambiamos.
*/
#ifndef SYNCUSRLIST_H
#define SYNCUSRLIST_H

#include <stdlib.h>
#include <glib.h>
#include <semaphore.h>	/*para los mutex de sync*/
#include <stdbool.h>
#include <string.h>

#include "../debug.h"
#include "user.h"


#define SUL_MAX_STR_SIZE	100	/*maximo tamaño de string "compare*/

typedef struct _syncusrlist_t syncusrlist_t;


/* Crea una lista syncronizada de usuarios
	REQUIRES:
		Nada
	RETURNS:
		syncusrlist_t != NULL (si no hubo error)
		NULL => ERROR
*/
syncusrlist_t *syncusrlist_create (void);


/*Inserta un usuario. en caso de ser NULL no lo inserta. Tener en cuenta que
 *el usr NO debe ser liberado desde mas arriva ya que en caso de ser asi
 *quedara un puntero apuntando a cuqluier lado dentro de la lista.
 
	REQUIRES:
		self != NULL;
	ENSURES:
		size(self)!=0
	RETURNS:
		false == ERROR
*/		
bool syncusrlist_insert (syncusrlist_t *self, user_t *usr);


/*Funcion que devuelve el tamaño
	REQUIRES:
		self != NULL
	RETURNS:
		size
*/
unsigned int syncusrlist_get_size (syncusrlist_t *self);

/*!La politica que vamos a utilizar en estas funciones es:
 * en caso de que busquemos "NULL" entonces lo que hacemos es devolver
 * directamente NULL.
 * REQUIRES:
	    self != NULL
 *
 * RETURNS:
 * 	  NULL ==> no se encontro.
 * 	  user ==> si se encontro.
*/

/*buscamos un usuario por el nombre*/
user_t *syncusrlist_by_name (syncusrlist_t *self, const char *name);

user_t *syncusrlist_by_number (syncusrlist_t *self, const char *numb);

user_t *syncusrlist_by_nick (syncusrlist_t *self, const char *nick);

user_t *syncusrlist_by_dni (syncusrlist_t *self, const char *dni);




/*Funcion para borrar un usuario. (hace user_destroy)
	REQUIRES:
		self != NULL
*/
void syncusrlist_remove_first (syncusrlist_t *self);

void syncusrlist_remove_last (syncusrlist_t *self);

/*esta podria ser para eliminar un determinado usuario si es que existe*/
void syncusrlist_remove_user (syncusrlist_t *self, user_t * usr);

/*Destructor
	REQUIRES:
		self != NULL
	RETURNS:
		NULL
*/
void syncusrlist_destroy (syncusrlist_t *self);




#endif

