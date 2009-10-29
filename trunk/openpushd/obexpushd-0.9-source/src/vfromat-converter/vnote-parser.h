#ifndef VNOTE_PARSER_H
#define VNOTE_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>



#define VNP_TABLE_FILE		"vnote_table.txt"
#define VNP_RARE_CHARS		"C2 C3"
#define VNOTE_SPECIAL_CHAR	'='

typedef struct _vnParser vnParser_t;


/* Constructor */
vnParser_t * vnp_create(void);

/* Destructor. */
void vnp_destroy(vnParser_t * vnp);

/* Funcion que parsea los datos, transofrma una cadena de caracteres codificadas
 * a una decodificada.
 * REQUIRES:
 *	data != NULL
 *	vnp != NULL
 * RETURNS:
 *	decoded != NULL		if no error
 *	NULL			on error
 */
char * vnp_parsearse (vnParser_t * vnp, char * data);



/*! Debug */
void vnp_print(vnParser_t * vnp);

#endif
