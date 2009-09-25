/*! Esta va a ser la clase que define la entidad "usuario", simplemente
 * es una tupla que va a contener por el momento la direccion MAC y el nickname
 */
#ifndef CUSER_H
#define CUSER_H


#include <QString>
#include "../consts.h"
#include "../debug.h"


#define MAC_ADDR_SIZE 17	/* no es el tamaño verdadero, es el size "visual" */
class CUser{
	public:
		/* Constructor que pide de una los datos, en caso de que sean
		 * NULL los string no los copia.
		 * REQUIRES:
		 *	mac != NULL && mac->size == MAC_ADDR_SIZE / * cacaso * /
		 * NOTE: copia memoria, liberar con lo que se llamo 
		 */
		CUser (QString * nick, QString * mac);
		
		/* GETS
		 * NOTE: devuelve el puntero NO COPIA MEMORIA => NO liberarla
		 */
		const QString& getNick (void) const {return nick;};
		const QString& getMAC (void) const {return MAC;};
		
		/* Funcion que copia un usuario desde un puntero devolviendo
		 * una nueva instancia.
		 * NOTE: GENERA memoria
		 */
		CUser * copy(void);
		
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