#include "syncusrlist.h"

struct _syncusrlist_t {
	GList *list;	/*lista de glib donde se basa el el modulo*/
	sem_t mutex;	/*mutex :D*/
}


/*funcion que determina si dos strings son iguales, no requirer nada*/

static bool syncusrlist_strequals (const char* c1, const char* c2, size_t tam)
{
	return;
}


/*Esta funcion bien groncha nos va hacer mas "eficiente" y legible el codigo
 *a la hora de buscar algo. DE FORMA ATOMICA
 *
 * /user_func : funcion que nos dice que parametro buscar (nombre/nick/dni....etc)
 *		proviniente de user. POR ESO NO DEBE SER MODIFICADO user.h
 *
 * /find : cadena de caracteres con la que vamos hacer el macheo
 *
 *	REQUIRES:
 		NO SE MODIFIQUE user.h :)
 		self != NULL
 		find != NULL
 	RETURNS:
 		user_t = NULL (si no se encontro o hubo error)
 		user_t != NULL caso contrario.
*/
static user_t *syncusrlist_sfind (syncusrlist *self, (const char*) (*usr_func) (user_t *), const char* find)
{
	user_t *result = NULL;
	GList *aux = NULL;	/*lo vamos a usar para hacer una busqueda manual*/
	char *field = NULL;
	bool mustContinue = true;

	ASSERT (self != NULL);
	ASSERT (find != NULL);
	
	if (self != NULL) {
		/*vamos a buscar de forma "atomica" para evitar problemas de sync*/
		if (sem_wait (&self->mutex) != 0)
			pdebug ("Error tomando el mutex");
		else {
			/*!ZONA CRITICA*/
			
			aux = g_list_first (self->list); /*obtenemos el ptr a la 1º celda*/
			
			while (aux != NULL && mustContinue) { 
				/*sabemos que mientras aux != NULL todavia quedan elementos*/
				/*!tomamos el elemento (no se si funcionara esto)*/
				result = (user_t *) aux->data;
				
				/*obtenemos el char * del user_t obtenido*/
				field = usr_func (result, result);
				mustContinue = syncusrlist_strequals (find, field, (size_t) SUL_MAX_STR_SIZE);
				
				if (mustContinue) {
					/*limpiamos un poco*/
					result = NULL;
					field = NULL;
				}
				aux = g_list_next(aux); /*obtenemos el proximo elemento*/
			}
			/*liberamos el semaforo*/
			if (sem_post (&self->mutex) != 0 )
				pdebug ("error al soltar el mutex"); 
		}
	}
	
	return result;
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
	c
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
	


user_t *syncusrlist_by_name (syncusrlist_t *self, const char *name)
{
	if (name == EMPTY) { /*no buscamos nada retornamos directamente*/
		pdebug ("recibimos name == NULL");
		return (user_t *) NULL;
	}
	
	ASSERT (self != NULL);	/*no nos importa porque vamos a llamar a syncusrlist_sfind*/
	/*aca vamos a usar la funcion interna syncusrlist_sfind*/
	return syncusrlist_sfind (self, &user_get_name, name);
}

user_t *syncusrlist_by_number (syncusrlist_t *self, const char *numb)
{
	if (numb == EMPTY) { /*no buscamos nada retornamos directamente*/
		pdebug ("recibimos numb == NULL");
		return (user_t *) NULL;
	}
	
	ASSERT (self != NULL);	/*no nos importa porque vamos a llamar a syncusrlist_sfind*/
	/*aca vamos a usar la funcion interna syncusrlist_sfind*/
	return syncusrlist_sfind (self, &user_get_number, numb);
}

user_t *syncusrlist_by_nick (syncusrlist_t *self, const char *nick)
{
	if (nick == EMPTY) { /*no buscamos nada retornamos directamente*/
		pdebug ("recibimos nick == NULL");
		return (user_t *) NULL;
	}
	
	ASSERT (self != NULL);	/*no nos importa porque vamos a llamar a syncusrlist_sfind*/
	/*aca vamos a usar la funcion interna syncusrlist_sfind*/
	return syncusrlist_sfind (self, &user_get_nick, nick);
}

user_t *syncusrlist_by_dni (syncusrlist_t *self, const char *dni)
{
	if (dni == EMPTY) { /*no buscamos nada retornamos directamente*/
		pdebug ("recibimos dni == NULL");
		return (user_t *) NULL;
	}
	
	ASSERT (self != NULL);	/*no nos importa porque vamos a llamar a syncusrlist_sfind*/
	/*aca vamos a usar la funcion interna syncusrlist_sfind*/
	return syncusrlist_sfind (self, &user_get_dni, dni);
}


/*Funcion para borrar un usuario.
	REQUIRES:
		usr == usuario a borrar (si es NULL no se hace nada)
		
*/
void syncusrlist_remove_first (syncusrlist_t *self)
{
	GList * aux = NULL;
	user_t *usr = NULL;
	
	ASSERT (self != NULL);
	
	if (self != NULL) {
		/*tomamos el semaforo*/
		if (sem_wait (&self->mutex) != 0) 
			pdebug ("error al tomar el mutex");
		else {
			/*!*******	zona critica	********/
			
			aux = g_list_first (self->list);
			if (aux != NULL) {
				/*obtenemos el usuario*/
				usr = (user_t*) aux->data;
				/*lo eliminamos de la lista*/
				self->list = g_list_remove (self->list, usr);
				/*lo liberamos de memoria*/
				usr = user_destroy (usr);
			}
			
			/*liberamos el semaforo*/
			if (sem_post (&self->mutex) != 0 )
				pdebug ("error al soltar el mutex"); 
		}
	}
	else
		pdebug ("syncusrlist == NULL");
	
}


void syncusrlist_remove_last (syncusrlist_t *self)
{
	GList * aux = NULL;
	user_t *usr = NULL;
	
	ASSERT (self != NULL);
	
	if (self != NULL) {
		/*tomamos el semaforo*/
		if (sem_wait (&self->mutex) != 0) 
			pdebug ("error al tomar el mutex");
		else {
			/*!*******	zona critica	********/
			
			aux = g_list_last (self->list);
			if (aux != NULL) {
				/*obtenemos el usuario*/
				usr = (user_t*) aux->data;
				/*lo eliminamos de la lista*/
				self->list = g_list_remove (self->list, usr);
				/*lo liberamos de memoria*/
				usr = user_destroy (usr);
			}
			
			/*liberamos el semaforo*/
			if (sem_post (&self->mutex) != 0 )
				pdebug ("error al soltar el mutex"); 
		}
	}
	else
		pdebug ("syncusrlist == NULL");
	
}

void syncusrlist_remove_user (syncusrlist_t *self, user_t * usr)
{
	ASSERT (self != NULL);
	
	if (self != NULL) {
		/*tomamos el semaforo*/
		if (sem_wait (&self->mutex) != 0) 
			pdebug ("error al tomar el mutex");
		else {
			/*!*******	zona critica	********/
			
			self->list = g_list_remove (self->list, usr);
			usr = user_destroy (usr);
			
			
			/*liberamos el semaforo*/
			if (sem_post (&self->mutex) != 0 )
				pdebug ("error al soltar el mutex"); 
		}
	}
	else
		pdebug ("syncusrlist == NULL");
}

/*Destructor
	REQUIRES:
		self != NULL
	RETURNS:
		NULL
*/
void syncusrlist_destroy (syncusrlist_t *self);
