#include <assert.h>
#include "userlist.h"

/*! DEBUG*/
#ifdef __DEBUG
void UserList::add_debug_user (const char* number)
{
	UserObject *user1 = NULL;
	QString *auxstr = NULL;
	user1 = new UserObject();
	if (user1 != NULL) {
		auxstr = new QString (number);
		user1->setNumber (*auxstr);
		delete auxstr; auxstr = NULL;
		this->insertUser (user1);
	}
	
}

#endif


UserList::UserList()
{
	this->list = NULL;
	
	this->list = new QList<UserObject*>();
	
	assert (this->list != NULL); /*nos aseguramos*/
	
	
	/*!DEBUG*/
#ifdef __DEBUG
	add_debug_user ("3516545682");
	add_debug_user ("+543516065926");
	add_debug_user ("+543515915105");
	add_debug_user ("156844728");
	add_debug_user ("+543515915105");
	add_debug_user ("+543515589908");
	add_debug_user ("+543512306042");
	add_debug_user ("156776035");
	add_debug_user ("+543513723491");
	add_debug_user ("+543516314985");
	this->printList();
#endif 
	
	
	
}

/*REQUIRES: usr != NULL*/
void UserList::insertUser (UserObject* usr)
{
	/*estamos seguros que list != NULL*/
	if (usr != NULL) {
		this->list->append (usr);
	}
	else
		pdebug ("recibimos un usr == NULL");
}
	
/*REQUIRES: usr != NULL*/
bool UserList::deleteUser (UserObject* usr)
{
	int i = 0;
	bool result = false;
	
	/*podriamos usar un iterator*/
	if (usr != NULL) {
		for (i = 0; i < this->list->size() && !result; ++i) {
			if (this->list->at(i) != NULL) {
				if (*this->list->at(i) == *usr) { /*usamos nuestro operador ==*/
					/*borramos el elemento*/
					delete (this->list->takeAt(i));
					result = true;
				}
			}	
		}
	}
	return result;
}

bool UserList::deleteUserptr (UserObject* usr)
{
	return this->list->removeOne(usr);
}

UserObject* UserList::getUserByName (const QString& name)
{
	int i = 0;
	UserObject *result = NULL;
	
	for (i = 0; i < this->list->size() && (result == NULL); ++i) {
		if (this->list->at(i) != NULL) {
			if (this->list->at(i)->getName() == name) {
				result = this->list->at(i);
			}
		}	
	}
	
	return result;
}
	
UserObject* UserList::getUserByNumber (const QString& number)
{
	int i = 0;
	UserObject *result = NULL;
	
	for (i = 0; i < this->list->size() && (result == NULL); ++i) {
		if (this->list->at(i) != NULL) {
			if (this->list->at(i)->getNumber() == number) {
				result = this->list->at(i);
			}
		}	
	}
	
	return result;
}

UserObject* UserList::getUserByNick (const QString& name)
{
	int i = 0;
	UserObject *result = NULL;
	
	for (i = 0; i < this->list->size() && (result == NULL); ++i) {
		if (this->list->at(i) != NULL) {
			if (this->list->at(i)->getNick() == name) {
				result = this->list->at(i);
			}
		}	
	}
	
	return result;
}

UserObject* UserList::getUserByDni (const QString& name)
{
	int i = 0;
	UserObject *result = NULL;
	
	for (i = 0; i < this->list->size() && (result == NULL); ++i) {
		if (this->list->at(i) != NULL) {
			if (this->list->at(i)->getDni() == name) {
				result = this->list->at(i);
			}
		}	
	}
	
	return result;
}
	
bool UserList::existNumber (const QString& n)
{
	return (getUserByNumber(n) != NULL);
}


void UserList::printList()
{
	int i = 0;
	
	/*borramos todos los usuarios dentro de la lista*/
	for (i = 0; i < this->list->size(); ++i) {
		if (this->list->at(i) != NULL) {
			this->list->at(i)->printUser();
		}	
	}
	
}

UserList::~UserList(void)
{
	int i = 0;
	
	/*borramos todos los usuarios dentro de la lista*/
	for (i = 0; i < this->list->size(); ++i) {
		if (this->list->at(i) != NULL) {
			delete this->list->at(i);
		}	
	}
	delete this->list;	/*borramos la lista interna*/
}


