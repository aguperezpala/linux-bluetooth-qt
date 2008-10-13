#include "filemanipulator.h"

FileManipulator::FileManipulator (qint64 txt, qint64 img)
{
	if (txt > 0 && img > 0) {
		this->maxTextFileSize = txt;
		this->maxImageFileSize = img;
	}
	else {
		/*!ACA DEBERIAMOS CARGAR DE ARCHIVO*/
		this->maxTextFileSize = 200;
		this->maxImageFileSize = 5000000;
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



void FileManipulator::parseFile (QString& str)
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

