/*! Esta "base de datos" va a estar implementada con una hash de usuarios, que
 * lo que va hacer es almacenar usuarios en el disco duro y en la hash.
 * La forma en que se van almacenar los archivos en el disco va a ser:
 * MAC,Nickname\n
 * Uno por fila, dividiendo la MAC del Nickname por el caracter ','
*/
#ifndef UDATABASE_H
#define UDATABASE_H

#include <stdio.h>	/* para almacenar los usuarios en disco */
#include <string.h>
#include <QString>
#include <QChar>
#include <QHash>
#include <QMutex>

#include "../cuser/cuser.h"
#include "../consts.h"
#include "../debug.h"


class UDataBase{
	public:
		/* Constructor: Se va a pedir un archivo de texto, el cual
		 * a medida que se vayan ingresando los usuarios se van a ir
		 * guardando en el mismo
		 * REQUIRES:
		 *	fname != NULL
		*/
		UDataBase (const char * fname);
		
		/* Funcion que chequea si existe un usuario o no.
		 * REQUIRES:
		 * 	user != NULL
		 * RETURNS:
		 *	true 	if exist
		 *	false 	otherwise
		 */
		bool existUser (const CUser * user); 
		
		/* Funcion que va a agregar a un usuario SI Y SOLO SI no se 
		 * encuentra en la base de datos, Si existe no lo agrega.
		 * REQUIRES:
		 * 	user != NULL
		 * RETURNS:
		 *	true 	si el usuario fue agregado
		 *	false	caso contrario (ej ya existia o error)
		 * NOTE: NO se debe liberar user despues de haber llamado a esta
		 * funcion.
		 */
		bool addUser (CUser * user);
		
		/* Funcion que carga una base de datos desde un archivo
		 * NOTE: Agrega a la lista ya existente, no BORRA lo que hay
		 * en la lista actual.
		 * REQUIRES:
		 *	fname != NULL
		 * RETURNS:
		 *	true 	if success
		 *	false 	otherwise
		 */
		bool loadFromFile (const char * fname);
		
		
		/* Guarda en el archivo abierto al principio de todo, esto para
		 * evitar inconsistencias de usuarios, tener repetidos y esas
		 * cosas.
		 */
		void saveToFile (void);
		
		
		/* Funcion que limpia la base de datos */
		void clean (void);
		
		/*!DEBUG*/
		#ifdef __DEBUG
		void print();
		#endif
		
		/* NOTE: libera toda la hash table */
		~UDataBase();
		
	private:
		
		/* Funcion que intenta agregar un usuario a la user database
		* fijandose que sean validas la MAC y NICK
		* RETURNS:
		* 	true 	if error
		*	false	if no error
		*/
		bool udb_add_user (QString * MAC, QString * nick);
		
		/* La tabla hash <key, value>*/
		QHash<QString, CUser*> hashTable;
		FILE * file;
		QString fname;
		QMutex mutex;
		
};

#endif