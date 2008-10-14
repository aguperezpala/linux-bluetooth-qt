#ifndef SMSOBJECT_H
#define SMSOBJECT_H
#include <QString>

class SmsObject
{
public:
	SmsObject();
	SmsObject(QString& m, QString& na, QString& nu, QString& d);
	
inline	QString* getMesg (){return &this->message;};
inline	QString* getName (){return &this->name;};
inline	QString* getNumber (){return &this->number;};
inline	QString* getDate (){return &this->date;};

inline	void setMesg (QString& m){this->message = m;};
inline	void setName (QString& n){this->name = n;};
inline	void setNumber (QString& n){this->number = n;};
inline	void setDate (QString& d){this->date = d;};


	
	
private:
	/*aca vamos a almacenar los datos de cada sms*/
	QString message;
	QString name;		/**/
	QString number;		/**/
	QString date;
	
};

#endif
