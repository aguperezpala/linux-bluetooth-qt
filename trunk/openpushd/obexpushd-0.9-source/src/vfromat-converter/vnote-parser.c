#include "vnote-parser.h"



struct _vnParser {
	GHashTable * hash;	/* donde vamos a tener el mapeo */
};


static int vnp_getline (char * line, FILE * f)
{
	int c = 0, i = 0;
	
	if (!line || !f)
		return -1;
	
	while ((c = fgetc(f)) != EOF && c != '\n') {
		line[i] = c;
		i++;
	}
	if (c != EOF) {
		line[i] = c; /* \n en teoria */
		line[i+1] = '\0';
	}
		
	
	return i;
}

/* Funcion que se va a encargar de parsear linea por liena, vamos a seguir el
 * siguiente formato, realChar<espacios en blancos>hexa\n
 * RETURNS:
 *	hexa en hexa
 *	realChar en realChar
 *	-1 on error
 *	0 si todo ta ok
 */
static int vnp_parse_line (char * line, char * hexa, char * realChar)
{
	int i = 0, j = 0;
	
	if (!line || !hexa || !realChar)
		return -1;
	
	/* el primer caracter en teoria es el realChar */
	realChar[0] = line[0];
	i++;
	while (line[i] != '\n') {
		if (strchr(VNP_ACCEPTED_CHARS, line[i])) {
			/* salteamos */
			hexa[j] = line[i];
			j++;
			i++;
			continue;
		}
		i++;
	}

	//printf("line: %s\tHexa: %s\tAscii: %s\n",line, hexa, realChar);
	return 0;
}


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
	char line[250] = {0};
	size_t lineReaded = 0;
	
	
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
	hexa = calloc(8, sizeof(*hexa));
	ascii = calloc(2, sizeof(*ascii));
	lineReaded = vnp_getline(line, file);
	/* Cargamos las cosas.. */
	while (lineReaded > 0 && vnp_parse_line(line, hexa, ascii) == 0) {
		/* convertimos a upper */
		vnp_convert_upper(hexa);
		/* ahora metemos todo en la hash */
		g_hash_table_insert(result->hash, hexa, ascii);
		printf ("Hexa: %s.\tChar: %s.\n", hexa, ascii);
		hexa = calloc(8, sizeof(*hexa));
		ascii = calloc(2, sizeof(*ascii));
		lineReaded = vnp_getline(line, file);
	}
	/* liberamos la basura */
	if (hexa)
		free(hexa);
	if (ascii)
		free (ascii);
	
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
char * vnp_parse (vnParser_t * vnp, char * data)
{
	char * result = NULL;
	int i = 0, j = 0;
	int size = 0, dataSize = 0;
	char buff[8] = {0};
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
			strcpy(result, data);
		return result;
	}
	/* si size != 0 es porque existe al menos un caracter, osea, existen
	 * size caracteres especiales, pero eso quiere decir que vamos a sacar
	 * 3 caracteres por size => tenemos size * 3 caracteres menos
	 */
	size = strlen(data);// - size * 3 + 2 /* por las dudas */;
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
			/* lo convertimos a upper case */
			vnp_convert_upper(buff);
			/*! debemos verificar si es un simbolo raro, si lo es
			 * entonces debemos generar un buffer distinto,
			 * agregando los 2 caracteres siguientes sin el "="
			 */
			if (strstr(VNP_RARE_CHARS, buff) != NULL) {
				/*! Es fucking raro => tomamos los proximos 3
				 * caracteres */
				memcpy(buff, &data[i], 6);
				/* aumentamos elos punteros */
				i = i + 5;
			} else
				i = i + 2;
			 
			/* buscamos el caracter correspondiente */
			realChar = g_hash_table_lookup(vnp->hash, buff);
			/* ahora guardamos el caracter verdadero en el buffer
			 * que vamos a devolver, */
			if (realChar)
				result[j] = realChar[0];
			
			/* limpiamos el buffer */
			memset(buff, '\0', 7);
		} else {
			/* no es un caracter especial => agregamos normalmente
			 * el caracter. */
			result[j] = data[i];
		}
		/* en cualquier caso aumentamos j */
		j++;
	}
	
	return result;
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
	
