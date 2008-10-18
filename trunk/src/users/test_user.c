/*testeo de user y userlist*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../debug.h"
#include "user.h"
#include "syncusrlist.h"

#define print(x)	printf("\n" x "\n")
#define printstr(x,y)	printf(x ": $%s$\n",y)
#define IFNULL(x)	if(x==NULL) pdebug("Null aca");	/*if null*/
#define IFNNULL(x)	if(x!=NULL) pdebug("Not null aca"); /*if not null*/
/*
static void check_user_fields (user_t * usr)
{
	const char * aux = NULL;
	
	aux = user_get_name (usr);
	printstr("name",aux);
	aux = NULL;
	
	aux = user_get_nick (usr);
	printstr("nick",aux);
	aux = NULL;
	
	aux = user_get_dni (usr);
	printstr("dni",aux);
	aux = NULL;
	
	aux = user_get_number (usr);
	printstr("number",aux);
	aux = NULL;
}

static void check_user_module (void)
{
	user_t * usr = NULL;
	
	print("Chequing user null");
	usr = user_new (NULL,NULL,NULL);
	IFNULL(usr)
	print("Chequing destroy");
	usr = user_destroy (usr);
	IFNNULL(usr)
	
	print("Chequing normal user");
	usr = user_new ("Agustin","Agu","3516545682");
	IFNULL(usr)
	check_user_fields (usr);
	
	
	
	print ("Chequing long fields");
	user_set_name (usr, "peasdlkawu0q23ue0a9sidasdañsidañoisu39oawu3ai");
	user_set_nick (usr, "aaaaaaaaaaaaaaaa123ad12345345346567568789fghcvbxzc");
	user_set_number (usr, "aaaaaaaaaaaaazxcccc,maskjd3y41i23yiashdkjahsd");
	user_set_dni (usr, "aaaaaaaaaaaaaazsdlkajsñlie34u09841ealksjdlñkajsd");
	check_user_fields (usr);
	
	
	print ("Chequing Nick name");
	usr = user_destroy (usr);
	usr = user_new ("Agustin",NULL,"3516545682");
	check_user_fields(usr);
	
	
	usr = user_destroy (usr);
	
	
}*/
int main (void) 
{
	user_t * usr = NULL;		/*objetos de debug*/
	syncusrlist_t * usrlist = NULL;
	user_t *aux = NULL;
		
	
	/*check_user_module();*/
	
	
			
	IFNULL(usrlist);
	
	aux = NULL;
	
	print("encolando usuarios");
	usr = user_new ("Agustin Perez Paladini", "Agu", "3516545682");
	usrlist = syncusrlist_create (usr);
	printf ("cola size: %d\n",(int) syncusrlist_get_size(usrlist));
	
	usr = user_new ("Juan loco", "Juan", "12345678");
	syncusrlist_insert (usrlist, usr);
	usr = user_new ("Mario Pepe", "marito", "48888666");
	syncusrlist_insert (usrlist, usr);
	usr = user_new ("Andre Lorenzon", "Andrews", "9998887545");
	syncusrlist_insert (usrlist, usr);
	printf ("cola size: %d\n",(int) syncusrlist_get_size(usrlist));
	syncusrlist_pprint (usrlist);
		
	/*print("Buscando usuario by name (buscando name = Juan loco)");
	aux = syncusrlist_by_name (usrlist,"Juan loco");
	IFNULL(aux);
	check_user_fields(aux);
	
	print("Buscando usuario by number (buscando 4888886666)");
	aux = syncusrlist_by_number (usrlist,"48888666");
	check_user_fields(aux);
	
	
	print("Buscando usuario by nick (buscando nick = Andrews)");
	aux = syncusrlist_by_nick (usrlist,"Andrews");
	check_user_fields(aux);
	*/
	/*print ("borrando el primer elemento");
	syncusrlist_remove_first (usrlist);
	syncusrlist_pprint (usrlist);
	
	print ("borrando el ULTIMO elemento");
	syncusrlist_remove_last (usrlist);
	syncusrlist_pprint (usrlist);
	*/
	print ("Borrando marito");
	aux = syncusrlist_by_nick (usrlist,"marito");
	syncusrlist_remove_user (usrlist, aux);
	syncusrlist_pprint (usrlist);
	printf ("cola size: %d\n",(int) syncusrlist_get_size(usrlist));
	
	usrlist = syncusrlist_destroy (usrlist);
	printf ("cola size: %d\n",(int) syncusrlist_get_size(usrlist));
	return 0;
}
