#include "smsobject.h"

SmsObject::SmsObject ()
{
}

SmsObject::SmsObject(QString& m, QString& na, QString& nu, QString& d)
{
	this->message = m;
	this->name = na;
	this->number = nu;
	this->date = d;
}


void SmsObject::print ()
{
	if (!this->message.isEmpty())
		dprintf ("Mensaje: %s \n",this->message.toStdString().c_str());
	if (!this->name.isEmpty())
		dprintf ("Name: %s \n",this->name.toStdString().c_str());
	if (!this->number.isEmpty())
		dprintf ("Number: %s \n",this->number.toStdString().c_str());
		
}

