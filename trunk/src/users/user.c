#include "user.c"


/*!vamos a hacerlo todo con estructuras dinamicas... cuidado :D*/
struct _user_t {
	char *dni;	
	char *name;
	char *nick;
	char *number;
	char *date;
};




user_t *user_new (char *name, char *nick, char *num);

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



