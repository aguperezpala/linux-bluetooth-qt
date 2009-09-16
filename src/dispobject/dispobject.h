/*! En este modulo se van a transportar los objetos que se van a mostrar en
 * pantalla (texto o imagen). Esta diferencia se va a reflejar por un campo
 * interno que va a determinar de que tipo es (un enum)
 */
#ifndef DISPOBJECT_H
#define DISPOBJECT_H

#include <QString>

#include "../cuser/cuser.h"
#include "../consts.h"
#include "../debug.h"

/*! Determina que tipo de objeto es */
typedef enum {
	DISPOBJ_TEXT, 		/* es de tipo TEXTO */
	DISPOBJ_PICTURE		/* es de tipo imagen */
} dispObjKind_t;


class DispObject {
	public:
		/* Atributos globales */
		dispObjKind_t kind;	/*! determina de que tipo es el obj */
		
		/* Constructor */
		DispObject (void);
		
		
		/* Funcion que se encarga de setear el usuario del dispobj
		 * Ademas elimina un usuario anterior si es que tiene.
		 * REQUIRES:
		 * 	usr != NULL
		 * NOTE: NO se debe liberar el usr una vez asignado.
		 */
		void setUser(CUser * usr);
		
		/* Funcion que devuelve el usuario del objeto.
		 * RETURNS:
		 *	NULL 	si no tiene usuario
		 *	user	caso contrario.
		 * NOTE: NO se debe liberar el usuario obetnido
		 */
		const CUser * getUser(void);
		
		/* Funcion que setea la data del object
		 * REQUIRES:
		 *	d.isNull() == false
		 * NOTE: SE copia la informacion.
		 */
		void setData (QString & d);
		
		/* Funcion que devuelve los datos.
		 * NOTE: Queda a criterio de quien la toma que se modifique o
		 *	 no a la informacion.
		 */
		QString & getData();
		
		/* Destructor: Libera el user */
		~DispObject();
		
	private:
		CUser * user;
		QString data;	/*! vamos a administrar el tipo de dato aca,
				 * en caso de ser tipo de texto guardamos
				 * directamente los datos a mostrar, en
				 * el caso de la imagen sera el nombre del 
				 * archivo,
				 */
		
		
};

#endif