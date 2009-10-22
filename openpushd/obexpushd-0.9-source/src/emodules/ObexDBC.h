#ifndef OOBEXDBC_H
#define OOBEXDBC_H


/* Se busca en la base de datos la dirección MAC pasada como parámetro
 * Si la misma está registrada. De ser así se retorna el nombre de usuario
 * asociado. Sino se devuelve NULL.
 *
 * PRE: MAC != NULL
 *
 * CALL: usr = existUser (MAC)
 *
 * POS: usr != NULL && "El nombre de usuario asociado a MAC es 'usr'"
 *	  OR
 *	usr == NULL &&  ("No hay conexión con la BD"
 *			   OR
 *			 "El cliente no está registrado en la BD")
 */
char *existUser (char *MAC);

#endif
