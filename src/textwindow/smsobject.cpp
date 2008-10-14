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