/*! 
 * Modulo que genera codigos y chequea que los codigos esten bien generados,
 * NOTE: Usa la fecha del sistema (deben estar corrdinadas si se usa en 2 pcs
 * distintas.
 * 
 * TESTED = false;
 *
 *
*/
#ifndef BTCODEADMIN_H
#define BTCODEADMIN_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
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

class BTCodeAdmin {
	
	public:
		/* constructor */
		BTCodeAdmin(void){};
		
		/* Funcion que determina si un codigo es valido o no 
		* RETURNS:
		* 	true	if code its valid
		* 	false	otherwise
		*/
		bool isCodeValid(string &code);
		
		/* Funcion que determina si un codifo fue ya usado o no
		 * RETURNS:
		 * 	true	if was used
		 * 	false 	otherwise
		 */
		bool wasUsed(string &code);
		
		/* Funcion que agrega un codigo ya usado a la lista de codigos
		 * usados
		 * REQUIRES:
		 * 	code is valid
		 */
		void addUsedCode(string &code);
		
		/* Funcion que setea un codigo como no usado... */
		void setCodeUnused(string &code);
		
		/* Funcion que genera un N codigos teniendo en cuenta un tamaño 
		* pre seteado (BTCG_CODE_SIZE).
		* RETURNS:
		* 	< 0	on error
		* 	0	if success
		*/
		int genCodes(vector<string> &codes, int N);
		
		/* Funcion que guarda la lista de codigos usados hasta
		 * el momento en un archivo
		 * NOTE: Rescribe el archivo si existe
		 */
		void toFile(string &fname);
		
		/* Funcion que carga una lista de codigos usados desde un
		 * archivo determinado.
		 */
		void fromFile(string &fname);
		
		/* destructor */
		~BTCodeAdmin(void);
		
	private:
		
		/* Converts a hexadecimal string to integer
		* REQUIRES:
		* 	xs != NULL
		* 	result != NULL
		* RETURNS
		* 	0    - Conversion is successful
		* 	1    - String is empty
		*	2    - String has more than 8 bytes
		* 	4    - Conversion is in process but abnormally terminated by 
		* 		illegal hexadecimal character
		*/
		int xtoi(const char* xs, unsigned int* result);
		
		
		
		map<string,bool> hash;
	
};
#endif
