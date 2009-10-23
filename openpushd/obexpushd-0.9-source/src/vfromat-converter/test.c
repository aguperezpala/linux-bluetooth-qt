#include "vformat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	data = clean_vnote (result);
	
	free (result);
	printf ("\n Codigo normal: \n %s\n", data);
	
	free (data);
	vformat_free(vf);
	
	return 0;
}
	