/*! Esta va a ser la clase que define la entidad "usuario", simplemente
 * es una tupla que va a contener por el momento la direccion MAC y el nickname
 */
#ifndef CUSER_H
#define CUSER_H

#include <QString>
#include "../debug.h"

class CUser{
	public:
		/* Constructor que pide de una los datos, en caso de que sean
		 * NULL los string no los copia.
		 * REQUIRES:
		 *	mac != NULL && mac->size == 17 / * cacaso * /
		 * NOTE: copia memoria, liberar con lo que se llamo 
		 */
		CUser (QString * nick, QString * mac);
		
		/* GETS
		 * NOTE: devuelve el puntero NO COPIA MEMORIA => NO liberarla
		 */
		inline const QString& getNick (){return this->nick;};
		inline const QString& getMAC (){return this->MAC;};
		
		
		/*! VAMOS A DETERMINAR QUE 2 USUARIOS SON IGUALES POR EL NUMERO
		  * DE MAC */
		/* REQUIRES: CUser other != NULL */
		bool operator==(const CUser & other);
		/* REQUIRES: CUser other != NULL */
		bool operator!=(const CUser & other);
		
		/*!DEBUG*/
		#ifdef __DEBUG
		void printUser();
		#endif
		
		~CUser();
		
	private:
		/*aca vamos a definir los campos de la clase usuario*/
		QString nick;
		QString MAC;
		
};

#endif