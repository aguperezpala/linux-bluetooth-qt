#include "vnote-parser.h"
#include <stdio.h>
#include <stdlib.h>



int main (int argc, char ** str) 
{
	vnParser_t * vnp = vnp_create();
	
	printf("\nImprimimos ahora por debug\n");
	vnp_print(vnp);
	vnp_destroy(vnp);
}
	