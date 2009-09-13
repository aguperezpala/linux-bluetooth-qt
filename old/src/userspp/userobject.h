#ifndef USEROBJECT_H
#define USEROBJECT_H

#include <QString>
#include "../debug.h"

class UserObject{
public:
    UserObject();

    /*!REQUIRES: param != NULL*/
    /*nos aseguramos de sacarle los espacios y esas cosas*/
inline void setName (QString& name){this->name = name;this->name.simplified();};
inline void setNumber (QString& num){this->number = num;this->number.simplified();};
inline void setDni (QString& dni){this->dni = dni;this->dni.simplified();};
inline void setNick (QString& nick){this->nick = nick;this->nick.simplified();};
inline void setDate (QString& date){this->date = date;this->date.simplified();};

inline const QString& getName (){return this->name;};
inline const QString& getNick (){return this->nick;};
inline const QString& getDni (){return this->dni;};
inline const QString& getNumber (){return this->number;};
inline const QString& getDate (){return this->date;};


	/*!VAMOS A DETERMINAR QUE 2 USUARIOS SON IGUALES POR EL NUMERO DE TELEFONO*/
	bool operator==(const UserObject&);
	bool operator!=(const UserObject&);
	
	/*!DEBUG*/
	void printUser();
	
    ~UserObject();

private:
	/*aca vamos a definir los campos de la clase usuario*/
	QString name;
	QString dni;
	QString nick;
	QString number;
	QString date;

};

#endif
