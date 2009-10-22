#include <stdio.h>
#include <assert.h>
#include "hash_keys.h"


int main (void)
{
	hashKeys_t * hk = NULL;
	
	hk = hash_k_create();
	
	hash_k_load_from_file(hk, "claves");
	/* existen */
	assert(hash_k_exist(hk, "petardo"));
	assert(hash_k_exist(hk, "agustin"));
	assert(hash_k_exist(hk, "pepeloc"));
	assert(hash_k_exist(hk, "trolona"));
	assert(hash_k_exist(hk, "queseio"));
	assert(hash_k_exist(hk, "ultimac"));
	assert(hash_k_exist(hk, "1234567"));
	/* no existen */
	assert(!hash_k_exist(hk, "muchos5"));
	assert(!hash_k_exist(hk, "agustia"));
	assert(!hash_k_exist(hk, "pepel"));
	
	hash_k_remove(hk, "petardo");
	hash_k_remove(hk, "agustin");
	hash_k_remove(hk, "trolona");
	hash_k_remove(hk, "queseio");
	hash_k_remove(hk, "saa");
	
	/* no existen */
	assert(!hash_k_exist(hk, "petardo"));
	assert(!hash_k_exist(hk, "trolona"));
	assert(!hash_k_exist(hk, "queseio"));
	assert(!hash_k_exist(hk, "saa"));
	
	hash_k_save_to_file(hk, "claves-result");
	
	
	hash_k_destroy(hk);
	
	return 0;
}
	
	