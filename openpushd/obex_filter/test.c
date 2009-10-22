#include <stdio.h>
#include <assert.h>
#include "obex_filter.h"


int main (void)
{
	/* accepted */
	assert(obf_always_accpted_file("prueba","512"));
	assert(obf_always_accpted_file("prueba.txt","412"));
	assert(obf_always_accpted_file("prueba.vnt","412"));
	/* not accepted */
	assert(!obf_always_accpted_file("prueba.txt","712"));
	assert(!obf_always_accpted_file("prueba.vnt","712"));
	assert(!obf_always_accpted_file("prueba","712"));
	assert(!obf_always_accpted_file("prueba.txt.a","312"));
	assert(!obf_always_accpted_file("prueba.vnt.a","312"));
	assert(!obf_always_accpted_file("prueba.a","312"));
	assert(!obf_always_accpted_file("prueba.ntv","312"));
	
	
	
	return 0;
}
	
	