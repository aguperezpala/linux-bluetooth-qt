#include "hash_keys.h"




struct _hashKeys {
	GHashTable *	hashTable;
	pthread_mutex_t	mutex;		/* para que soporte concurrencia */
};


static void hk_destroy_key (gpointer data)
{
	if (data)
		free(data);
}


/* Constructor */
hashKeys_t * hash_k_create (void)
{
	hashKeys_t * result = NULL;
	
	result = malloc(1 * sizeof (*result));
	if (!result)
		return result;
	result->hashTable = g_hash_table_new_full(g_str_hash, g_str_equal,
						   hk_destroy_key, NULL);
	pthread_mutex_init (&result->mutex, NULL);
	
	return result;
}

/* Destructor
* REQUIRES:
*	hk != NULL
*/
void hash_k_destroy (hashKeys_t * hk)
{
	if (!hk)
		return;
	if (hk->hashTable)
		/*! en teoria se libera todo aca... si no es dinamico */
		g_hash_table_destroy(hk->hashTable);
	pthread_mutex_destroy(&hk->mutex);
}

/* Funcion que va a cargar la hashKey desde un archivo.
* REQUIRES:
*	hk != NULL
*	fname != NULL
* NOTE: si ya existia la clave la reemplaza.
*/
void hash_k_load_from_file (hashKeys_t * hk, const char * fname)
{
	FILE * file = NULL;
	size_t size = 0;
	char * line = NULL;
	
	if (!fname || !hk)
		return;
	
	/* abrimos el archivo */
	if ((file = fopen(fname, "r")) == NULL) {
		perror ("Error al intentar abrir el archivo");
		return;
	}
	/* tenemos el archivo abierto, ahora vamos a leer linea por linea */
	while (getline(&line, &size, file) > 0) {
		/* vamos a sacar el '\n' */
		if (line && size){
			line[size] = '\0';
			/* agregamos a la hash table */
			g_hash_table_insert(hk->hashTable, line, line);
		}
		/* ahora que agregamos reinicializamos */
		line = NULL; size = 0;
	}
	
	fclose (file);
}

/* Funcion que va a guardar las claves que quedan en un archivo. Si el archivo
*  ya existia este es borrado y creado de nuevo.
* REQUIRES:
*	hk != NULL
*	fname != NULL
*/
void hash_k_save_to_file (hashKeys_t * hk, const char * fname)
{
	FILE * file = NULL;
	GHashTableIter iter;
	gpointer key, value;
	
	if (!fname || !hk)
		return;
	
	/* abrimos el archivo */
	if ((file = fopen(fname, "w")) == NULL) {
		perror ("Error al intentar abrir el archivo");
		return;
	}
	
	
	g_hash_table_iter_init(&iter, hk->hashTable);
	
	while (g_hash_table_iter_next (&iter, &key, &value)) 
	{
		if (key)
			fprintf(file, "%s\n", key);
	}
	
	/* cerramo el archivo */
	fclose(file);
}
	

/* Funcion que verifica si existe una clave determinada.
* REQUIRES:
*	hk != NULL
*	key != NULL
* RETURNS:
*	true	if exists
*	false	otherwise
*/
bool hash_k_exist (hashKeys_t * hk, const char * key)
{
	bool result = false;
	
	if (!key || !hk)
		return result;
	
	result = g_hash_table_lookup(hk->hashTable, key) != NULL;
	
	return result;
}


/* Funcion que elimina una clave determinada, si no existe no hace nada.
* REQUIRES:
*	hk != NULL
*	key != NULL
*/
void hash_k_remove (hashKeys_t * hk, const char * key)
{
	if (!key || !hk)
		return result;
	
	g_hash_table_remove(hk->hashTable, key);
}

