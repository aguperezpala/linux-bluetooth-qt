#include "dispobject.h"


/* Constructor: Toma el nombre del archivo
* REQUIRES:
*	fname.isNull() == false
*	file.exists() == true (file.setFileName(fname))
*/
DispObject::DispObject (QString & fname)
{
	/* pres */
	ASSERT (fname.isNull () == false);
	this->file.setFileName (fname);
	ASSERT (this->file.exists());	/*! existe el archivo ? */
	if (this->file.size() <= FILE_TEXT_MAX_SIZE) {
		QByteArray data = "";
		QString realData = "";
		
		/* el archivo entonces es de texto... */
		this->kind = DISPOBJ_TEXT;
		/* seteamos en el data el contenido */
		
		/* abrimos el archivo y lo lemos */
		if (this->file.open (QIODevice::ReadOnly)) {
			data = this->file.readAll();
			/* lo limpiamos de basuras como \n */
			data.replace ('\n', ' ');
			/* del tipo texto */
			realData = data;
			setData(realData);
			
			this->file.close();
		}
		
	} else
		/* el archivo es de imagen.. ### cuidado con esto... ### */
		this->kind = DISPOBJ_PICTURE;
	
	this->user = NULL;
}

DispObject::DispObject(void)
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
const QString & DispObject::getData() const
{
	return this->data;
}

void DispObject::deleteFile (void)
{
	if (this->file.exists(this->file.fileName()))
		/* si existe => eliminamos el archivo */
		if (!this->file.remove())
			/* error borrando el archivo?... */
			debugp ("DispObject::~DispObject: error al intentar "
			"borrar el archivo...:(\n");
}
/* Destructor: 
 * Libera el user 
 * NOTE: elimina el archivo si tiene asociado alguno
 */
DispObject::~DispObject()
{
	if (this->user != NULL)
		delete this->user;
	if(this->file.exists())
		deleteFile();
}