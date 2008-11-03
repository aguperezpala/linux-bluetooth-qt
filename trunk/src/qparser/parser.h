/* Esta clase la vamos a usar para extraer informacion sobre los archivos, y para
 * parsear paths y nombres de archivos
 */
#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include "../debug.h"



/*el final de sms siempre va a ser 2 endl*/
#define PARSER_END_SMS	"\n\n"

/*!Modificar esto*/
#define PARSER_BEGIN_SMS	"[SMSBackup"

/*aca vamos a definir los "campos" que queremos obtener desde el archivo*/
/*!	cada campo va a ser FIELD = xxxxx	*/


#define PARSER_NUMBER_FIELD	"Number"	/*hasta encontrar un \n*/
#define PARSER_TEXT_FIELD	";"			/*hasta encontrar un \n y despues un char!=';'*/


class Parser
{
public:
	Parser();
	
	/*esta funcion va a tomar el primer sms del QString source y ELIMINARLO
	de source una vez sacado el sms.
	
		RETURNS:
				NULL == ERROR
				first sms != NULL
	
	Nota: GENERA MEMORIA... tener en cuenta
	*/
	QString* getFirstSms (QString& source);
	
	/*!Estas funciones nos van a devolver determinados campos del sms.
		REQUIRES:
				QString != NULL
		RETURNS:
				NULL == ERROR
				field != NULL
	NOTA: GENERA MEMORIA... debe ser liberada luego
	*/
	
	/*obtenemos el numero limpio (sin comillas ni espacios)*/
	QString* getNumberField (QString *sms);
	
	/*obtenemos el texto*/
	QString* getTextField (QString *sms);
	
	
	private:
	
};

#endif
