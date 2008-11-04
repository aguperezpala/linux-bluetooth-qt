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
	void insertUser (UserObject* usr);
	
	/* elimina la memoria del usuerobject
		REQUIRES: usr != NULL
	*/
	
	bool deleteUser (UserObject* usr);
	bool deleteUserptr (UserObject* usr);
	
	
	/*!devolvemos NULL si no se encontro
	 * ESTAS FUNCIONES SON LAS DUSQUEDA*/
	UserObject* getUserByName (const QString& name);
	UserObject* getUserByNumber (const QString& n);
	UserObject* getUserByNick (const QString& n);
	UserObject* getUserByDni (const QString& d);
	
	bool existNumber (const QString&);
	
	
	/*!DEBUG*/
	void printList();



	/*!borramos todos los usuarios dentro de la lista*/
	~UserList(); 

private:
	QList<UserObject*> *list;

};

#endif
