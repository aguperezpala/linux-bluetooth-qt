#ifndef SMSOBJECT_H
#define SMSOBJECT_H
#include <QString>

#include "../debug.h"

class SmsObject
{
public:
	SmsObject();
	SmsObject(QString& m, QString& na, QString& nu, QString& d);
	
inline	const QString* getMesg (){return &this->message;};
inline	const QString* getName (){return &this->name;};
inline	const QString* getNumber (){return &this->number;};
inline	const QString* getDate (){return &this->date;};

/*! Todo QString que se le pase a estas funciones debe ser liberado luego 
	(si es posible) 
*/
inline	void setMesg (QString& m){this->message = m;};
inline	void setName (QString& n){this->name = n;};
inline	void setNumber (QString& n){this->number = n;};
inline	void setDate (QString& d){this->date = d;};


	/*!DEBUG*/
	void print();
	
private:
	/*aca vamos a almacenar los datos de cada sms*/
	QString message;
	QString name;		/**/
	QString number;		/**/
	QString date;
	
};

#endif
