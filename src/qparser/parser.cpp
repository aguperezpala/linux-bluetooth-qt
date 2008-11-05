#include "parser.h"

Parser::Parser ()
{

}

QString* Parser::getFirstSms (QString& source)
{
	QString *result = NULL;
	int begin = -1, end = -1;
	
		
	if (!source.isNull() && !source.isEmpty()) {
		/*ahora buscamos el principio del sms*/
		begin = source.indexOf (QString (PARSER_BEGIN_SMS), 0, Qt::CaseInsensitive);
		
		if (begin > 0)	/*tomamos el final del "sms"*/
			end = source.indexOf (QString (PARSER_END_SMS), begin, Qt::CaseInsensitive);
		
		if (end > begin) {
			/*ahora debemos generar el result*/
			result = new QString (source.mid (begin, end - begin));
			/*y borramos del source el sms*/
			source.remove (begin, end - begin);
		}
	}
	
	return result;
}


/*obtenemos el numero limpio (sin comillas ni espacios)*/
QString* Parser::getNumberField (QString *sms)
{
	QString *result = NULL;
	int begin = -1, end = -1;
	QString aux(PARSER_NUMBER_FIELD);
	/*sabemos que sms != NULL*/
	ASSERT (sms != NULL);
	
	if (sms != NULL && !sms->isEmpty()) {
		aux.append (" =");
		begin = sms->indexOf (aux, 0, Qt::CaseInsensitive);
		if (begin > 0){
			begin = begin + aux.length();
			end = sms->indexOf (QChar ('\n'), begin);
		}
		
		if (end > begin) {
			result = new QString (sms->mid (begin, end - begin));
			/*actualizamos la posicion de begin*/
			printf ("length:%d\t-%s-\n",result->length(),result->toStdString().c_str());
			if (result != NULL) {
				result->append (QChar ('\0'));
				/*reemplazamos las comillas*/
				result->replace (QChar ('"'), QString (""));
				/*sacamos los espacios*/
				result->replace (QString(" "), QString (""));
				result->simplified();
				
			}
		}
	}
	return result;
}

/*obtenemos el texto*/
QString* Parser::getTextField (QString *sms)
{
	QString *result = NULL;
	int begin = -1, end = -1;
	QString aux(PARSER_TEXT_FIELD);
	QChar c = '0';
	/*sabemos que sms != NULL*/
	ASSERT (sms != NULL);
	
	if (sms != NULL && !sms->isEmpty()) {
		begin = sms->indexOf (aux, 0, Qt::CaseInsensitive);
		
		if (begin > 0) {
			/*ahora debemos chequear que el proximo char sea distinto de ';'*/
			end = sms->indexOf (QString ("\n"), begin, Qt::CaseInsensitive);
			c = (*sms)[end+1];
			while (c == ';') {
				end = sms->indexOf (QString ("\n"), end+1, Qt::CaseInsensitive);
				c = (*sms)[end+1];
			}	
			/*en teoria ya tenemos el tamaño del texto, desde begin hasta end*/
			if (end > begin) {
				result = new QString ("");
				if (result != NULL) {
					/*agregamos el numero*/
					result->append (sms->mid (begin, end - begin));
					/*reemplazamos los ';'*/
					result->replace (QString(";"), QString (""));
					/*tambien sacamos los \n*/
					result->replace (QChar('\n'), QString (""));
				}
			}
		}
	}
	return result;
}


