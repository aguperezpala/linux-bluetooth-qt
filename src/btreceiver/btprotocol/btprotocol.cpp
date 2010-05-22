#include "btprotocol.h"

/* funcion que chequea si cmd es un comando valido
 * RETURNS:
 * 	cmd_num		if cmd its a valid command
 *	< 0		otherwise
 */
static int btproto_check_cmd(string &cmd)
{
	int i = 0;
	
	while(BT_CMD_STR[i].size() != 0) {
		if (cmd.compare(BT_CMD_STR[i])== 0)
			/* es igual, entonces si es un commando */
			return i;
		i++;
	}
	
	/* no encontramos ningun match */
	return -1;
}

/* Funcion que parsea la info recibida si y solo si respeta el protocolo
* RETURNS:
* 	< 0	si hay error o no se esta cumpliendo el protocolo
* 	0	si la cadena esta completa y correcta
*	1	parece que esta recibiendo correctamente.
*/
int btproto_parse_data(string &data, int &cmd, int &size, string &msg)
{
	string aux = "";
	int pos = 0;
	
	
	/* verificamos que tengamos algo que parsear */
	if (data.size() < BT_CMD_STR[0].size())
		/* no tenemos nada para verificar */
		return 1;
	
	/* vamos a chequear primero que si tenemos alguno de los commandos
	 * en los primeros bytes, si no tenemos un error de protocolo */
	aux = data.substr(0,4);
	cmd = btproto_check_cmd(aux);
	if (cmd < 0)
		/* no es un commando valido */
		return -1;
	
	/* extraemos el tamaño ahora */
	pos = data.find(':', 5);
	if (pos <= 0){
		/* debemos ver ahora si el tamaño de data es mayor que
		 * 20, lo cual nos dice que es un error de protocolo */
		if (data.size() > 20)
			return -1;
		else
			/* pareciera que estamos recibiendo bien los datos */
			return 1;
	}
	
	aux = data.substr(5, pos - 5);
	size = atoi(aux.c_str());
	if (size < 0)
		/* no es un entero? wtf... */
		return -1;
	
	/* si estamos aca tenemos un tamaño verdadero en size, hacemos el ultimo
	 * chequeo */
	if (data.size() < 4 + 2 + aux.size() + size)
		/*! no tenemos todos los datos, notar que si es mayor entonces
		 * puede ser que tengamos basura despues de los bytes
		 * que recibimos, no deberia suceder esto.
		 */
		return 1;
	
	/* esta todo correcto */
	msg = data.substr(pos + 1, size);
	
	
	return 0;
}
