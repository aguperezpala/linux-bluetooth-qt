#include "user.c"


/*!vamos a hacerlo todo con estructuras dinamicas... cuidado :D*/
struct _user_t {
	char *dni;	
	char *name;
	char *nick;
	char *number;
	char *date;
};


/* Result != NULL... osea que genera memoria*/
static char* user_get_first_name (char *name)
{
	ASSERT (name != NULL);
	
}

/*vamos a utilizar esta funcion interna para setear los campos (ya sea
 *sobreescribiendo los datos de field si es que existen), devolviendo
 *true en caso de poder haber seteado correctamente, false en caso contrario*/
 
static bool user_set_field (char *field, char * data, int max_field_size)
{
	bool result = FALSE;
		
	/*primero chequeamos que data != NULL*/
	if (data == NULL) {
		/*si es null entonces vamos a setear field = NULL*/
		if (field != NULL)
			free (field);
		field = NULL;
		return false;
	}
	else {	/*estamos en data != NULL */
		if (field == NULL) {
			/*generamos memoria para field*/
			ASSERT (max_field_size > 0);
			field = (char *) calloc (max_field_size, sizeof (char));
			ASSERT (field != NULL);
			if (field != NULL)
				/*copiamos los datos*/
				strncpy (data, field, max_field_size);
			else
				pdebug ("Error...");
		}
		
		
		
		
	
}

/*! Para simplificarnos la vida lo que vamos hacer es determinar el tamaño
 * por defecto, osea el MAX_... para cada parte de la estructura
*/
user_t *user_new (char *name, char *nick, char *num)
{
	user_t *result == NULL;
		
	result = (user_t *) calloc (1, sizeof (struct _user_t));
	
	ASSERT (result != NULL);	/*para debug*/
	
	if (result != NULL) {
		/*ahora vamos a chequear los parametros*/
		if (name != NULL) { /*agregamos el nombre*/
			
			result->name = (char *) calloc (USR_MAX_NAME_SIZE, sizeof (char));
			ASSERT (result->name != NULL);
			
			if (result->name != NULL)
				/*copiamos n caracteres*/
				strncpy (name, result->name, USR_MAX_NAME_SIZE);
		} else
			pdebag ("name == NULL");
		
		if (nick != NULL) {
			result->nick = (char *) calloc (USR_MAX_NICK_SIZE, sizeof (char));
			ASSERT (result->nick != NULL);
			
			if (result->nick != NULL)
				strncpy (nick, result->nick, USR_MAX_NICK_SIZE);
			
		} else {
			pdebug ("nick == NULL");
			/*vamos a obtener el primer "nombre" de NAME si se puede*/
			if (name != NULL)
				result->nick = user_get_first_name (name);
		}
		
			
			
			
		

	
}

/*! Para estas funciones vamos a tener en cuenta que si estamos intentando
 * setear algun tipo de dato a la estructura "self" y self == NULL entonces
 * vamos a devolver false
 */


bool user_set_name (user_t *self, char *name);	/*usamos la politica de arriva*/

bool user_set_nick (user_t *self, char *nick);	/*usamos la politica de arriva*/

bool user_set_number (user_t *self, char *num);	/*usamos la politica de arriva*/

bool user_set_dni (user_t *self, char *dni);	/*usamos la politica de arriva*/

bool user_set_date (user_t *self, char *date);	/*usamos la politica de arriva*/


/*!Para get vamos a implementar la politica de que si pedimos algun campo, puede
 * suceder que no este definido, por lo que vamos a devolver  NULL si es que no
 * existe, en caso contrario vamos a devolver el puntero al campo.
 */ 


const char* user_get_name (user_t *self);	/*usamos la politica de arriva*/

const char* user_get_nick (user_t *self);	/*usamos la politica de arriva*/

const char* user_get_number (user_t *self);	/*usamos la politica de arriva*/

const char* user_get_dni (user_t *self);	/*usamos la politica de arriva*/


/*	REQUIRES:
		Nada.. si self == NULL ==> RETURN NULL
*/
user_t *user_destroy (user_t *self);



