/*! 
 * Modulo que genera codigos y chequea que los codigos esten bien generados,
 * NOTE: Usa la fecha del sistema (deben estar corrdinadas si se usa en 2 pcs
 * distintas.
 * 
 * TESTED = false;
 *
 *
*/
#ifndef BTCODEGENERATOR_H
#define BTCODEGENERATOR_H

#include <string>
#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>

/* definimos la cantidad de caracteres de los codigos */
#define BTCG_CODE_SIZE		8

/*! vamos a definir los codigos de la siguiente forma:
 * XXXYYYYY
 * Donde XXX va a ser el numero del codigo (numCode) generado y YYYYY va a ser 
 * el numero tal que es igual a: numDay * numMonth * numCode
 * Representado en hexadecimal, y va a tener que ser enviado como tal.
 *
 */

using namespace std;

/* Funcion que determina si un codigo es valido o no 
 * RETURNS:
 * 	true	if code its valid
 * 	false	otherwise
 */
bool btcg_is_valid_code(string &code);

/* Funcion que genera un N codigos teniendo en cuenta un tamaño pre
 * seteado (BTCG_CODE_SIZE).
 * RETURNS:
 * 	< 0	on error
 * 	0	if success
 */
int btcg_gen_code(vector<string> &codes, int N);

#endif
