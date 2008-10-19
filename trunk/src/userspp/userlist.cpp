#include <assert.h>
#include "userlist.h"

UserList::UserList()
{
	this->list = NULL;
	
	this->list = new QList<UserObject*>();
	
	assert (this->list != NULL); /*nos aseguramos*/
	
}

/*REQUIRES: usr != NULL*/
void UserList::InsertUser (UserObject* usr)
{
	/*estamos seguros que list != NULL*/
	if (usr != NULL) {
		this->list->append (usr);
	}
	else
		pdebug ("recibimos un usr == NULL");
}
	
/*REQUIRES: usr != NULL*/
bool UserList::DeleteUser (UserObject* usr)
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

bool UserList::DeleteUserptr (UserObject* usr)
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
	
UserObject* UserList::getUserByNumber (const QString& name)
{
	int i = 0;
	UserObject *result = NULL;
	
	for (i = 0; i < this->list->size() && (result == NULL); ++i) {
		if (this->list->at(i) != NULL) {
			if (this->list->at(i)->getNumber() == name) {
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
	
bool existNumber (const QString& n)
{
	return (getUserByNumber (n) != NULL);
}


UserList::~UserList()
{
}


