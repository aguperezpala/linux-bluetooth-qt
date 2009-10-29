#include "vformat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	FILE * out;
	char * name;
} file_data_t;


static void put_convert_vnote (file_data_t * fdata)
{	
	char * name = NULL;
	char * vnote = NULL;
	char * clearVnote = NULL;
	VFormat * vf = NULL;
	size_t size = 0;
	
	printf ("XX\n\n\n");
	if (!fdata)
		return;
	
	/* primero vamos a ver si es del tipo.vnote */
	name = fdata->name;
	if (!name)
		return;
	if (strstr(name, ".vnt") == NULL) {
		/* no es del tipo vnote */
		free (name);
		return;
	}
	printf ("XX\n\n\n");
	{
		char buff[512];
		fdata->out = fopen (name, "r");
		
		if (!fdata->out) {
			free (name);
			return;
		}
		size = fread_unlocked((void*)buff, 1, 512, fdata->out);
		buff[size] = '\0';
		printf ("XX\n\n\n");
		fclose (fdata->out);
		vf = vformat_new_from_string (buff);
	}
	if (!vf) {
		free (name);
		fclose (fdata->out);
		return;
	}
	printf ("XX\n\n\n");
	vnote = vformat_to_string(vf, VFORMAT_NOTE);
	printf ("VNOTE: %s\n\n", vnote);
	clearVnote = clean_vnote (vnote);
	printf ("XX\n\n\n");
	if (!clearVnote) {
		vformat_free(vf);
		fclose (fdata->out);
		return;
	}
	printf ("XX\n\n\n");
	/* ahora abrimos el archivo y guardamos el nuevo contenido */
	fdata->out = fopen (name, "w");
	
	if (fdata->out) {
		fwrite (clearVnote, 1, strlen (clearVnote), fdata->out);
		fclose (fdata->out);
	}
	printf ("XX\n\n\n");
	free (clearVnote);
	if (vnote)
		free(vnote);
	vformat_free(vf);
	printf ("XX\n\n\n");
	return;
}


int main (int argc, char ** str) 
{
	FILE * f = NULL;
	char buff[500] = {0};
	VFormat * vf = NULL;
	size_t size = 0;
	char * result = NULL;
	char * data = NULL;
	file_data_t fdata;
	
	
	if (argc < 2) {
		printf ("Error falta archivo\n");
		return 1;
	}
	f = fopen (str[1], "r");
	
	if (!f) {
		printf ("No existe el archivo\n");
		return 1;
	}
	fdata.name = str[1];
	
	size = fread_unlocked((void*)buff, 1, 499, f);
	buff[size] = '\0';
	vf = vformat_new_from_string (buff);
// 	if (!vf) {
// 		printf ("NO ES DEL TIPO VNOTE\n");
// 		return 1;
// 	}
// 	printf ("\n La vnote es: \n %s\n",buff);
// 	
// 	result = vformat_to_string(vf, VFORMAT_NOTE);
// 	/*if (!result) {
// 		printf ("RESULTTTT NO ES DEL TIPO VNOTE\n");
// 		return 1;
// 	}*/
// 	data = clean_vnote (result);
// 	
// 	free (result);
// 	/*printf ("\n Codigo normal: \n %s\n", data);*/
// 	
// 	free (data);
	vformat_free(vf);
	
	//put_convert_vnote (&fdata);
	
	return 0;
}
	