#include <stdio.h>
#include <assert.h>
#include "file_manipulator.h"

#define TEST_DATA	"esto es una fucking test"
#define P(x)		printf(x)
int main (void)
{
	FILE * dummy = NULL;
	char * path = NULL;
	
	assert(!fm_file_exists("prueba.txt"));
	P("no existe el archivo, bien\n");
	
	dummy = fopen ("prueba.txt", "w");
	if (!dummy) {
		printf ("error creando archivo\n");
		return 1;
	}
	fwrite (TEST_DATA, 1 , strlen (TEST_DATA), dummy);
	fclose (dummy);
	
	assert(fm_file_exists("prueba.txt"));
	printf("Existe, bien\n");
	path = fm_get_absolute_path ("prueba.txt");
	printf("path con archivo: %s\n", path);
	free (path);
	path = fm_get_absolute_path (NULL);
	printf("path SIN archivo: %s\n", path);
	free (path);
	
	fm_copy_file("prueba.txt","prueba.txt.bak");
	assert(fm_file_exists("prueba.txt.bak"));
	
	fm_delete_file ("prueba.txt");
	assert(!fm_file_exists("prueba.txt"));
	P("El archivo no existe, fue borrado, bien\n");
	
	return 0;
}
	
	