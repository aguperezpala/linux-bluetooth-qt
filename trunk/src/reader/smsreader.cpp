#include "smsreader.h"

SmsReader::SmsReader(MainTxtWindow* m)
{
	this->txtwindow = NULL;
	this->fifo = NULL;
	this->status = false;
	
	this->txtwindow = m;
	assert (this->txtwindow != NULL);
	
}


void SmsReader::startReading()
{
	QString str("");			/*buffer auxiliar*/
	SmsObject *smsobj = NULL;
	QString *sms = NULL;
	QString *number = NULL;
	QString *text = NULL;
	int c = 0;
	
	this->status = true;		/*habilitamos*/
	
	while (this->fifo != NULL && this->status) {
		c = fgetc (this->fifo);
		if (c == EOF || this->fifo == NULL) {
			/*lo mandamos al parser*/
			sms = parser.getFirstSms (str);
			if (sms != NULL) {
				/*tenemos el primer sms*/				
				while (sms != NULL && !sms->isEmpty()) {
					/*mientras tengamos sms's que leer los leemos*/
					smsobj = new SmsObject();	/*creamos el smsobj*/
					if (smsobj != NULL) {
						number = parser.getNumberField (sms);
						if (number != NULL) {
							/*agregamos numero al smsobj*/
							smsobj->setNumber (*number);
							delete number; number = NULL;
						}
						text = parser.getTextField(sms);
						if (text != NULL) {
							/*agregamos el mensaje*/
							smsobj->setMesg (*text);
							delete text; text = NULL;
						}
						delete sms; sms = NULL;
						
						sms = parser.getFirstSms (str);
						/*!ahora lo agregamos a la txtwindow*/
						this->txtwindow->getExternSms (smsobj);
						smsobj = NULL;
						
					}
				}
				delete sms;	sms = NULL;
			}
			/* lo volvemos a abrir*/
			if (!this->filename.isNull() && !this->filename.isEmpty())
				this->fifo = fopen (this->filename.toStdString().c_str(), "r");
		} else {
			/*agregamos un str*/
			str.append (QChar (c));
		}
	
	}
	
	this->status = false;		/*desabilitamos*/
	
}

bool SmsReader::setFile(const char *fname)
{
	bool result = true;
	
	if (this->fifo != NULL) {	/*si esta abierto lo cerramos*/
		fclose (this->fifo);
		this->fifo = NULL;
		this->status = false;
	}
	
	if (fname == NULL)
		return false;
	
	this->filename = QString (fname);
	this->fifo = fopen (fname, "r");	/*abrimos con permiso de lectura*/
	result = (this->fifo != NULL);		/*comprobamos que se haya abierto*/
	
	return result;
}

SmsReader::~SmsReader()
{
	if (this->fifo != NULL)
		fclose (this->fifo);
	
}


