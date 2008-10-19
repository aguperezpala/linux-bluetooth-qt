#include "filemanipulator.h"
#include "../debug.h"
/*MUY CHORO SACAR ESTO*/
#include <stdio.h>
#include <stdlib.h>

FileManipulator::FileManipulator (qint64 txt, qint64 img)
{
	if (txt > 0 && img > 0) {
		this->maxTextFileSize = txt;
		this->maxImageFileSize = img;
	}
	
	this->fi = new QFileInfo;
}


unsigned int FileManipulator::getFileType (const QString& fname)
{
	unsigned int result = FK_ERROR;
	QString str = fname;
	qint64 aux = 0;
	//vamos a determinar el tipo de archivo segun el tamaño
	if (!fname.isNull() && !fname.isEmpty()) {
		this->fi->setFile (fname); 
		
		if (this->fi->exists()) {
			aux = this->fi->size ();
			if (aux <= this->maxTextFileSize)
				result = FK_TEXT;
			else if (aux <= this->maxImageFileSize)
				result = FK_IMAGE;
		}
	}
	return result;
}



void FileManipulator::parseFilePath (QString& str)
{
	bool cond = false;
	int i = 0;
	
	if (!str.isNull() && !str.isEmpty()) {//chequeamos por las dudas
		while (!cond && i < (str.length()-1)) {
			cond = ((str[i] == QChar ('/')) && (str[(i+1)] != QChar ('/')));
			i++;
		}
		/*!REVISAR SI EXISTE ALGUN CONVERTIDOR DE FORMATO DIRECTO*/
		//en este punto deberiamos tener i = primera / del verdadero path
		
		if (i != str.length() -2) { //por las dudas que ya fue parseado
			str.remove (0, i-1); //sacamos la "basura"
			str.replace ("%20"," "); //reemplazamos los espacios
			str.replace ("%2B","+"); //son los +...
			str.replace ("\n", ""); //sacamos los "enters"
			str.trimmed (); //sacamos los espacios al final y al principio
		}
	}
	
}

QString* FileManipulator::getFileContent (QString& file) 
{
	/*! BIEN AL ESTILO C CAMBIARLO por el parser*/
	FILE * fp = NULL;
	char *fname = NULL;
	char *cresult = NULL;
	QString *result = NULL;
	
	
	cresult = (char *) calloc ((int)this->maxTextFileSize, sizeof (char));
	if (cresult == NULL)
		return NULL;
	/*convertimos en char**/
	fname = (char *) file.toStdString().c_str();
	dprintf ("el archivo se llama @%s@\n",fname);
	
	
	fp = fopen (file.toStdString().c_str(), "r");
	if (fp == NULL) {
		return NULL;
	}
	
	dprintf ("se abrio %s\n",fname);
	cresult = fgets (cresult,(size_t)this->maxTextFileSize, fp);
	
	result = new QString (cresult);
	fclose (fp);
	free (cresult);
	
	return result;
}

SmsObject* FileManipulator::parseSmsFromFile (QString& file)
{
	SmsObject *result = NULL;
	
	result = new SmsObject();
	
	ASSERT (result != NULL);
	
	if (result != NULL) {
		/*!eso por ahoar despues cambiarlo, osea parsear todos los datos
		 *del sms y agregarlos a la estructura
		*/
		result->setMesg (*getFileContent (file));
	}
	
	return result;
}


