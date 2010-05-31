/*!
 * TESTED = true;
 *
 *
*/
#ifndef BTPROTOCOL_H
#define BTPROTOCOL_H

#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
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


/* Funcion que determina si tenemos un dato incompleto (1), completo (0) o 
 * erroneo (-1). No parsea nada.
 */
int btproto_check_data(string &data);

/* Funcion que parsea la info recibida si y solo si respeta el protocolo
* RETURNS:
* 	< 0	si hay error o no se esta cumpliendo el protocolo
* 	0	si la cadena esta completa y correcta
*	1	parece que esta recibiendo correctamente.
 */
int btproto_parse_data(string &data, int &cmd, int &size, string &msg);


/* Funcion que genera un string en base a un comando y unos datos determinado
 * respetando el protocolo.
 * REQUIRES:
 * 	cmd € BT_CMD_*
 * RETURNS:
 * 	NULL		on error
 * 	protocoledData	if success
*/
string *btproto_create_data(string &msg, int cmd);


#endif
