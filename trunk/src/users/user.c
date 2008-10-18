#include "user.h"
#include <string.h>

/*!vamos a hacerlo todo con estructuras dinamicas... cuidado :D*/
struct _user_t {
	char *dni;	
	char *name;
	char *nick;
	char *number;
	char *date;
};


/* Result != EMPTY... osea que genera memoria*/
static char* user_get_first_name (char *name)
{
	unsigned int s = 0;
	char c = '\0';
	char *result = NULL;
	
	ASSERT (name != EMPTY);
	if (name != EMPTY) {
		for (s = 0; s < strnlen (name, USR_MAX_NAME_SIZE) && (c != ' ' || c != '\t' || c != '\0'); s++)
			c = name[s];
		
		result = (char *) calloc (s+2, sizeof (char));
		if (result != NULL)
			strncpy (result, name, s+1);
		
	}
		
	return result;
	
}

/*vamos a utilizar esta funcion interna para setear los campos (ya sea
 *sobreescribiendo los datos de field si es que existen)
 	RETURNS:
 		true ==> seteado correctamente
		false en caso contrario
*/
static bool user_set_field (char **field,const char * data, int max_field_size)
{
	bool result = false;
	size_t aux = 0;
/*	unsigned int i = 0;*/
	/*primero chequeamos que data != EMPTY*/
	if (data == EMPTY) {
		/*si es null entonces vamos a setear field = EMPTY*/
		if (*field != EMPTY)
			free (*field);
		*field = EMPTY;
		return false;
	}
	else {	/*estamos en data != EMPTY */
		if (*field == EMPTY) {
			/*generamos memoria para field*/
			ASSERT (max_field_size > 0);
			*field = (char *) calloc (max_field_size+1, sizeof (char));
			ASSERT (*field != EMPTY);
			if (*field != EMPTY) {
				/*copiamos los datos*/
				strncpy (*field, data, max_field_size);
				result = true;
			}
			else
				pdebug ("Error...");
		} else {
			/*data != EMPTY && field != EMPTY*/
			/*primero limpiamos el string por las dudas*/
			aux = strnlen (*field, (size_t) max_field_size);
			
			/*a lo negro*/
			/*for (i = 0; i < aux; i++) {
				*field[i] = '\0';
				printf ("aux :%d\n",(int)i);
			}*/
			/*ahora copiamos*/
			strncpy (*field, data, max_field_size);
			result = true;
		}
	}
	
	return result;

}

/*! Para simplificarnos la vida lo que vamos hacer es determinar el tamaño
 * por defecto, osea el MAX_... para cada parte de la estructura
*/
user_t *user_new (char *name, char *nick, char *num)
{
	user_t *result = NULL;
		
	result = (user_t *) calloc (1, sizeof (struct _user_t));
	
	ASSERT (result != NULL);	/*para debug*/
	
	if (result != NULL) {
		/*ahora vamos a chequear los parametros*/
		if (!user_set_field (&result->name, name, USR_MAX_NAME_SIZE))
			pdebug ("name == EMPTY");
		
		if (nick != EMPTY) {
			user_set_field (&result->nick, nick, USR_MAX_NICK_SIZE);
				
		} else {
			pdebug ("nick == EMPTY");
			if (name != EMPTY) result->nick = user_get_first_name (name);
		}
		if (!user_set_field (&result->number, num, USR_MAX_NUM_SIZE))
			pdebug ("num == EMPTY");
			
	}
		
	return result;
}

/*! Para estas funciones vamos a tener en cuenta que si estamos intentando
 * setear algun tipo de dato a la estructura "self" y self == EMPTY entonces
 * vamos a devolver false
 */


bool user_set_name (user_t *self, char *name)
{
	ASSERT (self != NULL);
	
	if (self != NULL)
		return user_set_field (&self->name, name, USR_MAX_NAME_SIZE);
	else
		return false;
}
	

bool user_set_nick (user_t *self, char *nick)
{
	ASSERT (self != NULL);
	
	if (self != NULL)
		return user_set_field (&self->nick, nick, USR_MAX_NICK_SIZE);
	else
		return false;
}

bool user_set_number (user_t *self, char *num)
{
	ASSERT (self != NULL);
	
	if (self != NULL)
		return user_set_field (&self->number, num, USR_MAX_NUM_SIZE);
	else
		return false;
}

bool user_set_dni (user_t *self, char *dni)
{
	ASSERT (self != NULL);
	
	if (self != NULL)
		return user_set_field (&self->dni, dni, USR_MAX_DNI_SIZE);
	else
		return false;
	
}

bool user_set_date (user_t *self, char *date)
{
	ASSERT (self != NULL);
	
	if (self != NULL)
		return user_set_field (&self->date, date, USR_MAX_DATE_SIZE);
	else
		return false;
}


/*!Para get vamos a implementar la politica de que si pedimos algun campo, puede
 * suceder que no este definido, por lo que vamos a devolver  EMPTY si es que no
 * existe, en caso contrario vamos a devolver el puntero al campo.
 */ 


const char* user_get_name (user_t *self)
{
	ASSERT (self != NULL);
	
	if (self == NULL)
		return EMPTY;
	
	return self->name;
}
	
	

const char* user_get_nick (user_t *self)
{
	ASSERT (self != NULL);
	
	if (self == NULL)
		return EMPTY;
	
	return self->nick;
}

const char* user_get_number (user_t *self)
{
	ASSERT (self != NULL);
	
	if (self == NULL)
		return EMPTY;
	
	return self->number;
}


const char* user_get_dni (user_t *self)
{
	ASSERT (self != NULL);
	
	if (self == NULL)
		return EMPTY;
	
	return self->dni;
}

const char* user_get_date (user_t *self)
{
	ASSERT (self != NULL);
	
	if (self == NULL)
		return EMPTY;
	
	return self->date;
}



user_t *user_destroy (user_t *self)
{
	ASSERT (self != NULL);
	if (self != NULL) {
		if (self->name != EMPTY) {
			free (self->name); self->name = EMPTY;
		}
		if (self->nick != EMPTY) {
			free (self->nick); self->nick = EMPTY;
		}
		if (self->dni != EMPTY) {
			free (self->dni); self->dni = EMPTY;
		}
		if (self->number != EMPTY) {
			free (self->number); self->number = EMPTY;
		}
		if (self->date != EMPTY) {
			free (self->date); self->date = EMPTY;
		}
		
		free (self); self = NULL;
		
	}
	
	return NULL;
}



