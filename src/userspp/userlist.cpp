#include <assert.h>
#include "userlist.h"
#include <string.h>

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
	this->file = USER_LIST_DEFAULT_NAME;
	
	/*!DEBUG*/
#ifdef __DEBUG
	add_debug_user ("3516545682");
	add_debug_user ("+543514596055");
	add_debug_user ("+543515915105");
	add_debug_user ("156844728");
	add_debug_user ("+543515915105");
	add_debug_user ("+543515589908");
	add_debug_user ("+543512306042");
	add_debug_user ("156776035");
	add_debug_user ("+543513723491");
	add_debug_user ("+543516314985");
	
#endif 
	
}

/*REQUIRES: usr != NULL*/
void UserList::insertUser (UserObject* usr)
{
	/*estamos seguros que list != NULL*/
	if (usr != NULL) {
		this->list->append (usr);
		/*! tener en cuenta aca que hacemos un append*/
		appendUserToFile (usr);
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


bool UserList::toFile(QString& fname)
{
	const char *fileName = NULL;
	FILE *fd = NULL;
	int i = 0;
	UserObject *userObj = NULL;
	
	/*obtenemos el nombre del archivo a guardar*/
	if (fname.isNull () || fname.isEmpty ())
		fileName = USER_LIST_DEFAULT_NAME;
	else
		fileName = fname.toStdString().c_str();
	
	this->file = fname; /*igualamos el nombre del archivo*/
	
	fd = fopen (fileName, "w");
	
	if (fd == NULL)
		return false;
	
	for (i = 0; i < this->list->size(); ++i) {
		if (this->list->at(i) != NULL) {
			userObj = this->list->at(i);
			fprintf (fd, USER_LIST_FILE_SEPARATOR_UP "\n");	/*separador inicial*/
			
			/*ahora imprimimos todos los campos en el archivo, en este caso, solamente
			vamos a usar NUMERO*/
			fprintf (fd, USER_LIST_NUMBER_FIELD	"=");
			fprintf (fd, "%s\n",(userObj->getNumber()).toStdString().c_str());
			
			fprintf (fd, USER_LIST_FILE_SEPARATOR_BT "\n");	/*separador final*/
			
		}
	}
	
	fclose (fd);
	
	return true;
}

bool UserList::toFile(void)
{
	QString dummy("");
	
	return this->toFile(dummy);
}

bool UserList::fromFile(QString& fname)
{
	FILE *fd = NULL;
	UserObject *userObj = NULL;
	char buff[100]; /*como mucho vamos a tener 100 caracteres en una linea*/
	QString line(""), key ("");
	bool startUser = false, endUser = false;
	QString *field = NULL;
	
	if (fname.isNull() || fname.isEmpty())
		return false;
	
	fd = fopen (fname.toStdString().c_str(), "r");
	if (fd == NULL)
		return false;
	
	this->file = fname; /*igualamos el nombre del archivo*/
	
	memset (buff, '\0', 100);
	/*ahora tenemos que ir leyendo linea por linea del archivo hasta el final*/
	
	while (fgets (buff, 100, fd) != NULL) {
		if (strlen (buff) > 0) {
			/*si hay algo que hacer entonces..*/
			if (!startUser) {
				/*si todavia no encontramos algun inicio de usuario*/
				if (strstr (buff, USER_LIST_FILE_SEPARATOR_UP)) {
					/*encontramos un inicio de usuario*/
					userObj = new UserObject();
					startUser = true;
					endUser = false;
					
				}
			} else if (!endUser) {
				if (strstr (buff, USER_LIST_FILE_SEPARATOR_BT)) {
					/*debemos agregar el usuario*/
					if (userObj != NULL) {
						insertUser (userObj);
						userObj = NULL;
						startUser = false;
						endUser = true;
					} else
						pdebug ("UserList::fromFile: Estamos intentando meter un userObj NULL\n");
				} else {
					/*debemos agregar el numero o lo que sea que estamos obteniendo
					buff*/
					line = "";
					line = QString (buff);
					/*! Aca agregamos solamente EL NUMERO, habria que chequear los 
					* otros campos (nick/name/dni....etc)
					*/
					key = USER_LIST_NUMBER_FIELD;
					field = this->parser.getDataFromKey (line, key);
					if (field != NULL) {
						if (userObj != NULL) {
							/*seteamos el numero*/
							userObj->setNumber (*field);
							pdebug ("UserList: agregando usuario from file\n");
						}
						delete field; field = NULL;
					} else {
						/*debemos liberar el userObj porque no tiene info adentro*/
						delete userObj; userObj = NULL;
					}
				}
			}
			/*limpiamos el buffer, con esto en teoria alcanza*/
			buff[0] = '\0';
		}
	}
	
	fclose (fd);
	
	return true;	
}

bool UserList::fromFile(void)
{
	QString dummy("");
	
	return this->fromFile(dummy);
}


bool UserList::appendUserToFile (UserObject* usr)
{
	FILE *fd = NULL;
	
	/*Guarda nescesaria*/
	if (this->file.isNull() || this->file.isEmpty() || usr == NULL)
		return false;
	
	fd = fopen (this->file.toStdString().c_str(), "a");
	if (fd == NULL)
		return false;
	
	fprintf (fd, "\n");	/*por las dudas*/
	fprintf (fd, USER_LIST_FILE_SEPARATOR_UP "\n");	/*separador inicial*/
	/*ahora imprimimos todos los campos en el archivo, en este caso, solamente
	vamos a usar NUMERO*/
	fprintf (fd, USER_LIST_NUMBER_FIELD	"=");
	fprintf (fd, "%s\n",(usr->getNumber()).toStdString().c_str());
	fprintf (fd, USER_LIST_FILE_SEPARATOR_BT "\n");	/*separador final*/
	
	pdebug ("Append terminado\n");
	fclose (fd);
	return true;
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


