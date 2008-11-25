/*!Lista de PUNTEROS a objectos*/
#ifndef USERLIST_H
#define USERLIST_H

#include <QString>
#include <QList>


#include "../debug.h"
#include "userobject.h"
#include "../qparser/parser.h"

/**************	FILE CONFIG	********************/
#define USER_LIST_DEFAULT_NAME	"userlist.default"

#define USER_LIST_FILE_SEPARATOR_UP	"<user_start>"
#define USER_LIST_FILE_SEPARATOR_BT	"<user_end>"

/*los campos van a tener la siguiente forma FIELD=dato*/
#define USER_LIST_NAME_FIELD	"Name"
#define USER_LIST_NUMBER_FIELD	"Number"
#define USER_LIST_NICK_FIELD	"Nick"
#define USER_LIST_DNI_FIELD		"DNI"


/***********************************************/

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
	
	inline int getSize (){return this->list->size();};
	
	/* Tener en cuenta que siempre vamos a sobreescribir cualquier archivo
	 Si:
		fname == NULL || fname == Empty ==> USER_LIST_DEFAULT_NAME
	*/
	bool toFile(QString& fname);	/*guarda la lista en un archivo*/
	bool toFile(void);				/*!POR CONVENIENCIA*/
	
	bool fromFile(QString& fname);	/*carga la lista desde un archivo*/
	bool fromFile(void);			/*!POR CONVENIENCIA*/
	
	/*!DEBUG*/
#ifdef __DEBUG
	void printList();
	void add_debug_user (const char* number);
#endif


	/*!borramos todos los usuarios dentro de la lista*/
	~UserList(); 

private:
	QList<UserObject*> *list;
	Parser parser;
};

#endif
