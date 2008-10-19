
#include "userobject.h"

UserObject::UserObject()
{
	this->dni = QString("");
	this->name = QString("");
	this->nick = QString("");
	this->number = QString("");
	this->date = QString("");
}

bool UserObject::operator==(const UserObject& other)
{
	return (this->number == other.number);	
}

bool UserObject::operator!=(const UserObject& other)
{
	return (this->number != other.number);
}


UserObject::~UserObject()
{
}


