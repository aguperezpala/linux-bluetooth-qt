/* Esta clase la vamos a usar para extraer informacion sobre los archivos,
 */
#ifndef FILEMANIPULATOR_H
#define FILEMANIPULATOR_H

#include <QString>
#include <QFileInfo>

#include "../qparser/parser.h"
#include "../textwindow/smsobject.h"

class FileManipulator
{
	public:
		enum  fileKind{
			FK_TEXT,
			FK_IMAGE,
			FK_ERROR
		};
				
		/*constructor que pide los tamaños maximos de los diferentes
		*tipos de archivos, 200 bytes para texto 5 megas para foto
		*/
		FileManipulator(qint64 maxtxt = 200, qint64 maximg = 5000000);
		
			/*esta funcion parsea el nombre de un archivo. Lo que hace es buscar
		*despde el principio hasta hasta que encuentra el ultimo / antes de
		*cualquier otra letra, y lo elimina, luego busca todo los %20 y los
		*reemplaza por espacios en blanco, devolviendo el nombre del archivo
		*sin espacios
		REQUIRES:
		str (path absoluto)
		"RETURNS:"
		remplaza str por path absoluto sin espacios
			*/
		void parseFilePath (QString&);
		
		/*Esta funcion nos va a decir que tipo de archivo es
		* SEGUN EL TAMAÑO DEL ARCHIVO! NO IMPORTA LA EXTENCION
		*	REQUIRES:
				param == parseado
		*/
		unsigned int getFileType (const QString&);
		
		/*	REQUIRES:
				file == parseado
			RETURN:
				NULL si hubo error
				QString con contenido caso contrario
		*/
		QString* getFileContent (QString& file);
		
		
		/*esta funcion nos parsea un sms desde un archivo
			REQUIRES:
				file != NULL
			RETURNS:
				NULL == ERROR
				SmsObject* en caso contrario
		*/
		SmsObject* parseSmsFromFile (QString& file);
		
		/*setear maximo tamaño*/
		void setMaxTxtSize (qint64 s){ maxTextFileSize = s; }
		void setMaxImgSize (qint64 s){ maxImageFileSize = s; }
		
	
	private:
		QFileInfo *fi;
		qint64 maxTextFileSize;
		qint64 maxImageFileSize;
};

#endif
