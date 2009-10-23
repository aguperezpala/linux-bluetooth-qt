#include "vformat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Funcion que va a sacar los '\n' y los "\n" que genera el vnote parser.
* REQUIRES:
*	data != NULL
* Convierte estos caracteres en espacios en blanco.
*/
static void vnote_replace_newlines (char * data)
{
	int i = 0;
	
	if (!data)
		return;
	
	
	while (data[i] != '\0') {
		if (data[i] == '\\') {
			/* tenemos una barra, veamos si lo que sigue, es una
			* 'n', si es asi => reemplazamos los 2 caracteres por
			* un espacio, aca deberiamos chequear que otro caracter
			* viene, si es una letra, la reemplazamos, si no no
			* degeriamos */
			data[i] = ' ';
			switch (data[i+1]) {
				case 'n':
					data[i+1] = ' ';
					break;
				case 't':
					data[i+1] = ' ';
					break;
				default:
					/* cualquier otro caso puede ser vaya 
					* a saber que, vamos a dejarlo :p 
					*/
					break;
			}
		}
		i++;
	}
}

/* Funcion que reparsea la vnote para simplemente sacar el string del contenido
* todo lo demas lo eliminamos
* REQUIRES:
*	vnote != NULL
* RETURNS:
*	clean_vnote	!= NULL on success
*	NULL		on error
*/
char * clean_vnote (const char * vnote)
{
	char * result = NULL;
	char * start = NULL, * end = NULL;
	int size = 0;
	
	if (!vnote)
		return NULL;
	
	/* verificamos cuanta memoria vamos a requerir */
	start = strstr(vnote, "BODY");
	end = strstr(vnote, "CATEGORIES");
	/*! veamos que esto esta agarrado con pinzas, deberiamos investigar
	* mas sobre esto.
	*/
	/* ahora vamos a buscar donde realmente empieza el mensaje */
	if (start)
		start = strchr(start, ':');
	
	/* verificamos si hay algun error o no */
	if (!start || !end || (start >= end) )
		return NULL;	/*! error */
		
		/* ahora calculamos el tamaño que vamos a necesitar */
		size = end - start - 1;
	/* pedimos memoria */
	result = calloc (size + 1, sizeof(*result));
	
	if (result) {
		/* copiamos la memoria */
		memcpy (result, start + 1, size);
		result[size] = '\0';
		/* ahora hacemos un reemplazo de los caracteres \n por espacios
		*/
		vnote_replace_newlines(result);
	}
	
	
	return result;
}
int main (int argc, char ** str) 
{
	FILE * f = NULL;
	char buff[500] = {0};
	VFormat * vf = NULL;
	size_t size = 0;
	char * result = NULL;
	char * data = NULL;
	
	
	
	if (argc < 2) {
		printf ("Error falta archivo\n");
		return 1;
	}
	f = fopen (str[1], "r");
	
	if (!f) {
		printf ("No existe el archivo\n");
		return 1;
	}
	
	size = fread_unlocked((void*)buff, 1, 499, f);
	buff[size] = '\0';
	vf = vformat_new_from_string (buff);
	printf ("\n La vnote es: \n %s\n",buff);
	
	result = vformat_to_string(vf, VFORMAT_NOTE);
	vnote_replace_newlines(result);
	data = clean_vnote (result);
	
	free (result);
	printf ("\n Codigo normal: \n %s\n", data);
	
	free (data);
	vformat_free(vf);
	
	return 0;
}
	