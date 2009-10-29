#include "vnote-parser.h"



struct _vnParser {
	GHashTable * hash;	/* donde vamos a tener el mapeo */
};


/* Funcion que va a contar la cantidad de ocurrencias de un caracter
 * REQUIRES:
 *	data != NULL
 * RETURNS:
 *	cantidad de ocurrencias del caracter
 */
static int vnp_count_char(char * data, char c)
{
	int result = 0;
	int i = 0;
	int size = 0;
	
	if (!data)
		return result;
	
	size = strlen(data);
	for (i = 0; i < size; i++)
		if (data[i] == c)
			result++;
	
	return result;
}

/* Funcion que convierte todos los caracteres de data a upper case.
 * REQUIRES:
 *	data != NULL
 */
static void vnp_convert_upper(char * data)
{
	int i = 0;
	int size = 0;
	
	if (!data)
		return;
	
	size = strlen(data);
	
	for (i = 0; i < size; i++)
		data[i] = toupper(data[i]);
}

/* Constructor */
vnParser_t * vnp_create(void)
{
	vnParser_t * result = NULL;
	FILE * file = NULL;
	char * hexa = NULL;
	char * ascii = NULL;
	
	
	result = calloc(1, sizeof(*result));
	if (!result)
		return NULL;
	
	result->hash = g_hash_table_new_full(g_str_hash, g_str_equal,free,free);
	if (!result->hash) { 
		free(result);
		result = NULL;
	}
	/*! ahora vamos a cargar desde el archivo las cosas */
	file = fopen (VNP_TABLE_FILE, "r");
	if (!file) {
		perror("Error al intentar abrir el archivo " VNP_TABLE_FILE "\n");
		vnp_destroy(result); result = NULL;
		return result;
	}
	hexa = calloc(3, sizeof(*hexa));
	ascii = calloc(2, sizeof(*ascii));
	/* Cargamos las cosas.. */
	while (fscanf(file, "%c%c\t%c\n", &hexa[0], &hexa[1], &ascii[0]) != EOF) {
		/* convertimos a upper */
		vnp_convert_upper(hexa);
		/* ahora metemos todo en la hash */
		g_hash_table_insert(result->hash, hexa, ascii);
		printf("Hexa: %s\tAscii: %s\n", hexa, ascii);
		hexa = calloc(3, sizeof(*hexa));
		ascii = calloc(2, sizeof(*ascii));
	}
	/* liberamos la basura */
	free(hexa);	free (ascii);
	
	fclose(file);
		
	
	return result;
}

/* Destructor. */
void vnp_destroy(vnParser_t * vnp)
{
	if (!vnp)
		return
	g_hash_table_destroy(vnp->hash);
	free(vnp);
}




/* Funcion que parsea los datos, transofrma una cadena de caracteres codificadas
* a una decodificada.
* REQUIRES:
*	data != NULL
* RETURNS:
*	decoded != NULL		if no error
*	NULL			on error
*/
char * vnp_parsearse (vnParser_t * vnp, char * data)
{
	char * result = NULL;
	int i = 0, j = 0;
	int size = 0, dataSize = 0;
	char buff[3] = {0};
	char * realChar = NULL;
	
	/* PRE */
	if (!vnp || !data)
		return NULL;
	
	/*! Lo que vamos a hacer, es ir leyendo hasta encontrar el caracter
	 * VNOTE_SPECIAL_CHAR, una vez que encontramos este, lo que hacemos
	 * es tomar los proximos 2, y reemplazar estos 3 caracteres (osea el
	 * VNOTE_SPECIAL_CHAR + los 2 siguientes HEXADECIMALES) por el ascii
	 * correspondiente.
	 */
	/* obtenemos el posible tamaño */
	size = vnp_count_char(data, VNOTE_SPECIAL_CHAR);
	if (size == 0) {
		/* simplemente devolvemos una copia */
		result = calloc(strlen(data) + 1, sizeof(char));
		if (result)
			strcpy(result, data)
		return result;
	}
	/* si size != 0 es porque existe al menos un caracter, osea, existen
	 * size caracteres especiales, pero eso quiere decir que vamos a sacar
	 * 3 caracteres por size => tenemos size * 3 caracteres menos
	 */
	size = strlen(data) - size * 3 + 2 /* por las dudas */;
	/* allocamos memoria */
	result = calloc(size, sizeof(*result));
	
	if (!result)
		return NULL;
	
	dataSize = strlen(data);
	/* ahora hacemos las transformaciones */
	for (i = 0; i < dataSize; i++) {
		if (data[i] == VNOTE_SPECIAL_CHAR) {
			/* obtenemos los caracteres hexas */
			memcpy(buff, &data[i+1], 2);
			/* buscamos el caracter correspondiente */
			realChar = g_hash_table_lookup(vnp->hash, buff);
			
	
	
}




/*! Debug */
void vnp_print(vnParser_t * vnp)
{
	GHashTableIter iter;
	gpointer key, value;
	
	if (!vnp)
		return;
	
	g_hash_table_iter_init (&iter, vnp->hash);
	while (g_hash_table_iter_next (&iter, &key, &value)) 
	{
		printf("Hexa: %s\tAscii: %s\n", (char *) key, (char *) value);
	}
}
	
