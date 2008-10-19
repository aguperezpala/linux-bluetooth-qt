/*!Lista de PUNTEROS a objectos*/
#ifndef USERLIST_H
#define USERLIST_H

#include <QString>
#include <QList>


#include "../debug.h"
#include "userobject.h"

class UserList{
public:
	UserList();
	/*REQUIRES: usr != NULL*/
	void InsertUser (UserObject* usr);
	
	/*REQUIRES: usr != NULL*/
	bool DeleteUser (UserObject* usr);
	bool DeleteUserptr (UserObject* usr);
	
	
	/*!devolvemos NULL si no se encontro*/
	UserObject* getUserByName (const QString& name);
	UserObject* getUserByNumber (const QString& n);
	UserObject* getUserByNick (const QString& n);
	UserObject* getUserByDni (const QString& d);
	




    ~UserList();

private:
	QList<UserObject*> *list;

};

#endif
