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
int BTCodeAdmin::xtoi(const char* xs, unsigned int* result)
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







/* Funcion que determina si un codigo es valido o no 
* RETURNS:
* 	true	if code its valid
* 	false	otherwise
*/
bool BTCodeAdmin::isCodeValid(string &code)
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

/* Funcion que determina si un codifo fue ya usado o no
* RETURNS:
* 	true	if was used
* 	false 	otherwise
*/
bool BTCodeAdmin::wasUsed(string &code)
{
	return (this->hash.find(code) != map::end);
}

/* Funcion que agrega un codigo ya usado a la lista de codigos
* usados */
void BTCodeAdmin::addUsedCode(string &code)
{
	this->hash.insert( pair<string,bool>(code,true) );
}

/* Funcion que setea un codigo como no usado... */
void BTCodeAdmin::setCodeUnused(string &code)
{
	this->hash.erase(code);
}

/* Funcion que genera un N codigos teniendo en cuenta un tamaño 
* pre seteado (BTCG_CODE_SIZE).
* RETURNS:
* 	< 0	on error
* 	0	if success
*/
int BTCodeAdmin::genCodes(vector<string> &codes, int N)
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

/* Funcion que guarda la lista de codigos usados hasta
* el momento en un archivo
* NOTE: Rescribe el archivo si existe
*/
void BTCodeAdmin::toFile(string &fname)
{
	fstream filestr;
	map<string,bool>::iterator it;
	string aux = "";
	
	filestr.open(fname.c_str(), fstream::out);
	
	if (!filestr.good())
		return;
	
	
	for(it = this->hash.begin(); it != this->hash.end(); ++it) {
		aux = (*it).first;
		aux.append('\n');
		filestr.write(aux.c_str(), aux.size());
		if(filestr.fail())
			break;
	}
	
	filestr.close();
}

/* Funcion que carga una lista de codigos usados desde un
* archivo determinado.
*/
void BTCodeAdmin::fromFile(string &fname)
{
	fstream filestr;
	char line[BTCG_CODE_SIZE + 2];
	int size = BTCG_CODE_SIZE + 2;
	string aux = "";
	
	filestr.open(fname.c_str(), fstream::in);
	
	if (!filestr.good())
		return;
	while(!filestr.fail()){
		filestr.getline(line, size);
		aux = line;
		if(!filestr.eof())
			this->hash.insert( pair<string,bool>(aux, true) );
		else
			break;
	}
	
	
	filestr.close();
}

/* destructor */
BTCodeAdmin::~BTCodeAdmin(void)
{
	
}


