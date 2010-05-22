#ifndef BTPROTOCOL_H
#define BTPROTOCOL_H

#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


using namespace std;
/*! vamos a definir los comandos posibles 
* NOTE: debe haber un matching con los strings */
static const string BT_CMD_STR[] = {
	"resp",
	"requ",
	"regi",
	"text",
	"file",
	"erro",
	""
};

enum {
	BT_CMD_RESP,
	BT_CMD_REQU,
	BT_CMD_REGI,
	BT_CMD_TEXT,
	BT_CMD_FILE,
	BT_CMD_ERRO
};


/* Funcion que parsea la info recibida si y solo si respeta el protocolo
* RETURNS:
* 	< 0	si hay error o no se esta cumpliendo el protocolo
* 	0	si la cadena esta completa y correcta
*	1	parece que esta recibiendo correctamente.
 */
int btproto_parse_data(string &data, int &cmd, int &size, string &msg);



#endif
