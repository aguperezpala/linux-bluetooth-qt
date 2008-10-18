/*testeo de user y userlist*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../debug.h"
#include "user.h"
#include "syncusrlist.h"

#define print(x)	printf(x "\n")
#define printstr(x,y)	printf(x ": $%s$\n",y)
#define IFNULL(x)	if(x==NULL) pdebug("Null aca");	/*if null*/
#define IFNNULL(x)	if(x!=NULL) pdebug("Not null aca"); /*if not null*/

static void check_user_fields (user_t * usr)
{
	const char * aux = NULL;
	
	aux = user_get_name (usr);
	IFNULL(aux)
	printstr("name",aux);
	aux = NULL;
	
	aux = user_get_nick (usr);
	IFNULL(aux)
	printstr("nick",aux);
	aux = NULL;
	
	aux = user_get_dni (usr);
	IFNULL(aux)
	printstr("dni",aux);
	aux = NULL;
	
	aux = user_get_number (usr);
	IFNULL(aux)
	printstr("number",aux);
	aux = NULL;
	
	aux = user_get_number (usr);
	IFNULL(aux)
	printstr("number",aux);
	aux = NULL;
	
	
}


int main (void) {
	user_t * usr = NULL;		/*objetos de debug*/
	/*syncusrlist_t * usrlist = NULL; */
	/*const char * aux = NULL;*/
	
	usr = user_new (NULL,NULL,NULL);
	IFNULL(usr)
	usr = user_destroy (usr);
	IFNNULL(usr)
	
	usr = user_new ("Agustin","Agu","3516545682");
	IFNULL(usr)
	
	print ("Chequing 1");
	check_user_fields (usr);
	
	print ("Chequing long fields");
	user_set_name (usr, "aaaaaaaaaaaaaaabbb33311122255555554444333");
	user_set_nick (usr, "aaaaaaaaaaaaaaabbb33311122255555554444333");
	user_set_number (usr, "aaaaaaaaaaaaaaabbb33311122255555554444333");
	user_set_dni (usr, "aaaaaaaaaaaaaaabbb33311122255555554444333");
	check_user_fields (usr);
	
	usr = user_destroy (usr);
	
	return 0;
}
