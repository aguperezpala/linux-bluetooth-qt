/*! Esta "base de datos" va a estar implementada con una hash de usuarios, que
 * lo que va hacer es almacenar usuarios en el disco duro y en la hash.
 * La forma en que se van almacenar los archivos en el disco va a ser:
 * MAC,Nickname\n
 * Uno por fila, dividiendo la MAC del Nickname por el caracter ','
 * Ademas tiene un servidor, que por el momento va a soportar consultas
 * via LAN
*/
#ifndef UDATABASE_H
#define UDATABASE_H

#include <stdio.h>	/* para almacenar los usuarios en disco */
#include <string.h>
#include <QString>
#include <QChar>
#include <QHash>
#include <QMutex>
#include <QThread>

#include "../cuser/cuser.h"
#include "../consts.h"
#include "../debug.h"
#include "../simple_server/sserver.h"
#include "../simple_server/sclient.h"
/*! en este archivo esta definido todo el protocolo SSDBP */
#include "udbserver/udbprotocol.h"


class UDataBase: public QThread{
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
		 * funcion.*//*! si el usuario ya existe es liberado lo mismo
		 */
		bool addUser (CUser * user);
		
		/* Funcion que devuelve un usuario si existe en la base de datos
		 * a partir de la MAC.
		 * REQUIRES:
		 *	MAC.isNull() == false
		 * RETURNS:
		 *	usr->copy()	si existe el usuario.
		 *	NULL		si no existe.
		 * NOTE: devuelve una copia => GENERA MEMORIA
		 */
		CUser * getUserFromMac (QString & MAC);
		
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
		
		/* Funcion que corre el servidor en determinados puertos. Esto
		* lo corre en un nuevo thread, por lo que no es bloqueante.
		* Esta funcion permite la posibilidad de accesos externos
		* a la base de datos respetando el protocolo <SSDBP>.
		* Vamos a escuchar en alguno de los puertos de rango definido
		* arriba.
		* NOTE: Si esta funcion es llamada y el servidor esta corriendo
		* 	no tiene efecto.
		*/
		void runServer (void);
		
		/* Funcion que cierra el servidor.
		 * NOTE: en caso de que no este corriendo, no tiene efecto.
		 */
		void stopServer (void);
		
		
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
		
		/* Funcion que practicamente carga el server y se ejecuta
		 * en el thread (es llamada desde runServer())
		 */
		void run (void);
		 
		
		/* La tabla hash <key, value>*/
		QHash<QString, CUser*> hashTable;
		FILE * file;
		QString fname;
		QMutex mutex;
		bool serverOn;
		
};

#endif