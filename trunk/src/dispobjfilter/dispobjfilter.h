/*! Este modulo es el filtro de los DispObjects, lo que hace:
 * 1) Chequea que pertenezca a un usuario que este registrado (en la db).
 * 1.a) Si no esta, borra el archivo asociado al DispObject y borra el 
 *	DispObject mismo liberando la memoria.
 * 2) 	Si esta registrado el usuario, entonces muestra al operador una ventana
 * 	con el contenido del archivo con la posibilidad de aceptar/ rechazar
 *	el mismo.
 * 2.a) Si es rechazado por el operador => (1.a).
 * 2.b) Si es aceptado, entonces se encola en la lista de elementos a mostrar.
*/
#ifndef DispObjFilter_H
#define DispObjFilter_H

#include <QString>
#include <QAbstractButton>
#include <QMessageBox>
#include <QFile>

#include "../udatabase/udatabase.h"
#include "../dispobject/dispobject.h"
#include "../debug.h"

class DispObjFilter
{

public:
	/* Constructor: Requiere de una base de datos donde pueda chequear
	 * si existen o no usuarios en un momento determinado.
	 * REQUIRES:
	 *	udb != NULL
	 */
	DispObjFilter(UDataBase * udb);
	
	
	/* Funcion principal que hace todo practicamente, es la que chequea
	 * si el owner (user) del DispObject pertenece a la udb, si es asi
	 * chequea si es aceptado por el operador. Si no, libera memoria
	 * y borra el archivo asociado.
	 * REQUIRES:
	 *	dobj != NULL
	 * RETURNS:
	 *	true	if it was accepted
	 *	false 	otherwise (NOTE: ensures: fileRmove(dobj) && 
	 *				free (dobj))
	 * NOTE: si devolvemos false dobj YA FUE LIBERADO! no se debe hacer un
	 * 	free del mismo.
	 */
	bool accept(DispObject * dobj);
	
	/* Destructor:*/
	~DispObjFilter();


private:
	/* ### 			Funciones 			### */
	
	/* Funcion que va a mostrar al usuario una ventana para aceptar/rechazar
	 * determinado dobj del tipo de dobj_texto.
	 * REQUIRES:
	 *	dobj 		!= NULL
	 *	dobj->kind	== DISPOBJ_TEXT
	 * RETURNS
	 *	true		if it was accepted
	 *	false		otherwise
	 * NOTE: NO se libera memoria.
	 */
	bool acceptTextObj (const DispObject * dobj);
	
	/* Funcion que va a mostrar al usuario una ventana para aceptar/rechazar
	* determinado dobj del tipo de dobj_picture.
	* REQUIRES:
	*	dobj 		!= NULL
	*	dobj->kind	== DISPOBJ_PICTURE
	* RETURNS
	*	true		if it was accepted
	*	false		otherwise
	* NOTE: NO se libera memoria.
	*/
	bool acceptPictureObj (const DispObject * dobj);
	
	UDataBase * udb;	/* base de datos de usuario :) */
	
};


#endif
