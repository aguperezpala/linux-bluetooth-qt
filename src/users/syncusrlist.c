#include "syncusrlist.h"

struct _syncusrlist_t {
	GList *list;	/*lista de glib donde se basa el el modulo*/
	sem_t mutex;	/*mutex :D*/
}




syncusrlist_t *syncusrlist_create (void)
{
	syncusrlist_t *result = NULL;
	
	result = (syncusrlist_t *) calloc (1, sizeof (struct _syncusrlist_t));
	ASSERT (result != NULL);

	if (result != NULL) {
		if (sem_init (&result->mutex, 0, 1) != 0) {
			/*no se pudo crear el semaforo*/
			pdebug ("No se pudo generar el semaphore");
			free (result);
			return NULL;
		}
		/*se genero correctamente*/
		result->list = g_list_alloc();	/*generamos la glist*/
	}
	
	return result;
}



/*Inserta un usuario. en caso de ser NULL no lo inserta.
	REQUIRES:
		self != NULL;
	ENSURES:
		size(self)!=0
	RETURNS:
		false == NO SE INSERTO (error)
*/		
bool syncusrlist_insert (syncusrlist_t *self, user_t *usr)
{
	bool result = false;

	if (usr == NULL) /*no agregamos nada*/
		return false;

	ASSERT (self != NULL);
	if (self != NULL) {
		/*tomamos el semaforo*/
		if (sem_wait (&self->mutex) != 0) 
			pdebug ("error al tomar el mutex");
		else {
			/********	zona critica	********/
			/* insertamos el usuario*/
			self->list = g_list_append (self->list, (gpointer) usr);
			result = true;
			/*liberamos el semaforo*/
			if (sem_post (&self->mutex) != 0 )
				pdebug ("error al soltar el mutex"); 
		}
		
	}
	/*ASSERT (syncusrlist_get_size (self) != 0); //en caso de que haya sido agregado*/
	return result;
}


unsigned int syncusrlist_get_size (syncusrlist_t *self)
{
	unsigned int result = 0;
	
	ASSERT (self != NULL);
	if (self != NULL) {
		/*tomamos el semaforo*/
		if (sem_wait (&self->mutex) != 0) 
			pdebug ("error al tomar el mutex");
		else {
			/********	zona critica	********/

			result = g_list_length (self->list);

			/*liberamos el semaforo*/
			if (sem_post (&self->mutex) != 0 )
				pdebug ("error al soltar el mutex"); 
		}
		
	}
	
	return result;
}
	

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
user_t *syncusrlist_by_name (syncusrlist_t *self, char *name);

user_t *syncusrlist_by_number (syncusrlist_t *self, char *numb);

user_t *syncusrlist_by_nick (syncusrlist_t *self, char *nick);

user_t *syncusrlist_by_dni (syncusrlist_t *self, char *dni);


/*Funcion para borrar un usuario.
	REQUIRES:
		usr == usuario a borrar (si es NULL no se hace nada)
		self != NULL
*/
void syncusrlist_remove (syncusrlist_t *self, user_t *usr);



/*Destructor
	REQUIRES:
		self != NULL
	RETURNS:
		NULL
*/
void syncusrlist_destroy (syncusrlist_t *self);
