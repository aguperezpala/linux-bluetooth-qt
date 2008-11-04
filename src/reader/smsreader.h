/*! Clase para leer desde el dispositivo "fifo" momentaneamete los Sms
*/
#ifndef SMSREADER_H
#define SMSREADER_H

#include <stdio.h>
#include <QString>

#include "../qparser/parser.h"
#include "../textwindow/smsobject.h"
#include "../textwindow/maintxtwindow.h"

class SmsReader {

public:
	
	bool status;	/*true == active | false == inactive*/
	
	/*constructor le pasamos la maintxtwindow a la que le vamos a mandar
	los mensajes parseados
		REQUIRES:
				m != NULL
	*/
	SmsReader(MainTxtWindow* m);
	
	/*setea el archivo del cual empezar a leer (cerrando el otro en caso de 
	estar abierto
		REQUIRES:
				fname != NULL
		RETURNS:
				true == open the new file
				false == error
	*/
	bool setFile(const char *fname);
	
	/*funcion que comienza a leer en un bucle infinito o hasta que se le
	diga que pare "stopReading()"
	*/
	void startReading();
	
	/*!fijarse la sincronizacion, no se detiene de leer instantaneamente*/
inline	void stopReading(){this->status = false;};
	
	
	
	~SmsReader();/*cerramos el archivo si es que esta abierto*/
	
	
	
	
private:
	FILE *fifo;
	MainTxtWindow *txtwindow;
	Parser parser;
};




#endif
