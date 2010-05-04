#include "vnote-parser.h"
#include <stdio.h>
#include <stdlib.h>

#define VNOTE_TEST "A=20B=20c=20d=20efjklmn=C3=B1opqrstuvwxyz=C3=A1=C3=A9=C3=AD=" \
"C3=B3=C3=BA=C3==BC=201234567890.,-!?@~_()<{}>[]=3D=C2=BF=C2=A1=C2=A4=C2=A2=" \
"C2=AB=C2=BB=C2==AE=C2=A9=C2=B0\/&';^|:=E2=82=AC$=C2=A3=C2=A7%=C2=A5*+#=0A"


int main (int argc, char ** str) 
{
	vnParser_t * vnp = vnp_create();
	char * vnoteParsed = NULL;
	
	printf("\nImprimimos ahora por debug\n");
	/*vnp_print(vnp);*/
	vnoteParsed = vnp_parse(vnp, VNOTE_TEST);
	
	if (vnoteParsed != NULL) {
		FILE * out = NULL;
		printf ("\n\nVerdaderos datos: %s\n\n", vnoteParsed);
		/* convertimos */
		out = fopen("out.test", "w");
		fwrite(vnoteParsed, 1, strlen(vnoteParsed), out);
		fclose(out);
		
		free (vnoteParsed);
	} else 
		printf ("\nNULLLL\n");
	
	vnp_destroy(vnp);
}
	