/* Las partes del "objeto" user las vamos a manejar todas con char* para facilitar
 * el trabajo de "matcheo"
 */
#ifndef USER_C_H
#define USER_C_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Practicamente estos defines no los vamos a tocar nunca*/
#define USR_MAX_DNI_SIZE	9
#define USR_MAX_NAME_SIZE	30
#define USR_MAX_NICK_SIZE	10	/*el nombre que se va a mostrar en pantalla*/
#define USR_MAX_NUM_SIZE	15	/*supongo que no hay mas que eso*/
#define USR_MAX_DATE_SIZE	20	/*registrar la fecha que se inscribio..*/

typedef struct _user_t user_t;






#endif
