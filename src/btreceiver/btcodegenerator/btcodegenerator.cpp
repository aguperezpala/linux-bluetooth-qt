#include "btcodegenerator.h"

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
static int xtoi(const char* xs, unsigned int* result)
{
	size_t szlen = 0;
	int i, xv, fact;
	
	assert(xs != NULL);
	assert(result != NULL);
	
	szlen = strlen(xs);
	
	if (szlen > 0)
	{
		// Converting more than 32bit hexadecimal value?
		if (szlen>BTCG_CODE_SIZE) return 2; // exit
			
		// Begin conversion here
		*result = 0;
		fact = 1;
		
		// Run until no more character to convert
		for(i=szlen-1; i>=0 ;i--)
		{
			if (isxdigit(*(xs+i)))
			{
				if (*(xs+i)>=97)
				{
					xv = ( *(xs+i) - 97) + 10;
				}
				else if ( *(xs+i) >= 65)
				{
					xv = (*(xs+i) - 65) + 10;
				}
				else
				{
					xv = *(xs+i) - 48;
				}
				*result += (xv * fact);
				fact *= 16;
			}
			else
			{
				// Conversion was abnormally terminated
				// by non hexadecimal digit, hence
				// returning only the converted with
				// an error value 4 (illegal hex character)
				return 4;
			}
		}
	}
	
	// Nothing to convert
	return 0;
}


/*! vamos a definir los codigos de la siguiente forma:
* XXXYYYYY
* Donde XXX va a ser el numero del codigo (numCode) generado y YYYYY va a ser 
* el numero tal que es igual a: numDay * numMonth * numCode
* Representado en hexadecimal, y va a tener que ser enviado como tal.
*
*/

/* Funcion que determina si un codigo es valido o no 
* RETURNS:
* 	true	if code its valid
* 	false	otherwise
*/
bool btcg_is_valid_code(string &data)
{
	time_t t = time(NULL);
	struct tm *lt = localtime(&t);
	string codeNumStr = "";
	string codeStr = "";
	unsigned int codeNum = 0, code = 0;
	unsigned int aux = 0;
	
	
	if (lt == NULL || data.size() != BTCG_CODE_SIZE)
		return false;
	
	codeNumStr = data.substr(0,3);
	codeStr = data.substr(3, data.size() - 3);
	/* obtenemos los codigos en valor numerico */
	if ((xtoi(codeNumStr.c_str(), &codeNum) != 0) ||
		xtoi(codeStr.c_str(), &code) != 0)
		return false;
	
	/*! verificamos que realmente sea un codigo el que tenemos 
	 * NOTE: aca esta el algoritmo  */
	
		
	aux = ((unsigned int) lt->tm_mon) * ((unsigned int) lt->tm_mday) *
		codeNum;
	if(aux == code)
		return true;
	
	return false;
}

/* Funcion que genera un codigo determinado teniendo en cuenta un tamaño pre
* seteado (BTCG_CODE_SIZE).
* RETURNS:
* 	< 0	on error
* 	0	if success
*/
int btcg_gen_code(vector<string> &codes, int N)
{
	unsigned int codeNum = 7, cod = 0;
	char result[BTCG_CODE_SIZE] = {0};
	string code = "";
	int i = 0, j = 0;
	time_t t = time(NULL);
	struct tm *lt = localtime(&t);
	
	if (lt == NULL)
		return -1;
	
	codes.clear();
	for (j = 0; j < N; j++) {
		code.clear();
		/* ahora vamos a agregar el numero y tantos 0 antes como hagan falta */
		sprintf(result, "%X", codeNum);
		for (i = 3 - strlen(result); i > 0; i--)
			code.append("0");
		
		code.append(result);
		memset(result, '\0', sizeof(result));
		
		/* ahora seteamos el valor del codigo */
		cod = ((unsigned int) lt->tm_mon) * ((unsigned int) lt->tm_mday) *
			codeNum;
		sprintf(result, "%X", cod);
		for (i = 5 - strlen(result); i > 0; i--)
			code.append("0");
		
		codeNum++;
		code.append(result);
		codes.push_back(code);
	}
	
	return 0;
}
