
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

void UserObject::printUser()
{
	printf ("Nombre: %s\n",this->name.toStdString().c_str());
	printf ("Nick: %s\n",this->nick.toStdString().c_str());
	printf ("Numero: %s\n",this->number.toStdString().c_str());
	printf ("Dni: %s\n",this->dni.toStdString().c_str());
	printf ("Date: %s\n",this->date.toStdString().c_str());
}

UserObject::~UserObject()
{
}


