#include "dispobject.h"


/* Constructor */
DispObject::DispObject (void)
{
	this->user = NULL;
}


/* Funcion que se encarga de setear el usuario del dispobj
* REQUIRES:
* 	usr != NULL
* NOTE: NO se debe liberar el usr una vez asignado.
*/
void DispObject::setUser(CUser * usr)
{
	ASSERT (usr != NULL);
	
	if (this->user != NULL)
		delete this->user;
	
	this->user = usr;
}

/* Funcion que devuelve el usuario del objeto.
* RETURNS:
*	NULL 	si no tiene usuario
*	user	caso contrario.
* NOTE: NO se debe liberar el usuario obetnido
*/
const CUser * DispObject::getUser(void)
{
	return this->user;
}

/* Funcion que setea la data del object
* REQUIRES:
*	d.isNull() == false
* NOTE: SE copia la informacion.
*/
void DispObject::setData (QString & d)
{
	ASSERT (d.isNull() == false);
	this->data = d;
}

/* Funcion que devuelve los datos.
* NOTE: Queda a criterio de quien la toma que se modifique o
*	 no a la informacion.
*/
QString & DispObject::getData()
{
	return this->data;
}
/* Destructor: Libera el user */
DispObject::~DispObject()
{
	if (this->user != NULL)
		delete this->user;
}