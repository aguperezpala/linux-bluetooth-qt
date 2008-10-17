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
 *a la hora de buscar algo. 
 *
 * /user_func : funcion que nos dice que parametro buscar (nombre/nick/dni....etc)
 *		proviniente de user. POR ESO NO DEBE SER MODIFICADO user.h
 *
 *	REQUIRES:
 		NO SE MODIFIQUE user.h :)
 		self != NULL
 	RETURNS:
 		user_t = NULL (si no se encontro o hubo error)
 		user_t != NULL caso contrario.
*/
static user_t *syncusrlist_sfind (syncusrlist *self, (const char*) (*usr_func) (user_t *))
{
	user_t *result = NULL;
	GList *aux = NULL;	/*lo vamos a usar para hacer una busqueda manual*/
	char *field = NULL;

	
	ASSERT (self != NULL);
	if (self != NULL) {
		aux = g_list_first (self->list); /*obtenemos el ptr a la 1º celda*/
		while (aux != NULL && result == NULL) { 
		/*sabemos que mientras aux != NULL todavia quedan elementos*/
		/*!tomamos el elemento (no se si funcionara esto)*/
		result = (user_t *) aux->data;
		
		/*obtenemos el char **/
		field = usr_func (result);

		

			
		
		
		
		
		


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
user_t *syncusrlist_by_name (syncusrlist_t *self, char *name)
{
	user_t *result = NULL;
	
	if (name == NULL) { /*no buscamos nada retornamos directamente*/
		pdebug ("recibimos name == NULL");
		return result;
	}
	
	ASSERT (self != NULL);
	if (self != NULL) {
		
		

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
