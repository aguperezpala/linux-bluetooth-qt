#include "dispobjfilter.h"


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
bool DispObjFilter::acceptTextObj (const DispObject * dobj)
{
	bool result = false;
	QMessageBox *msgFile = NULL;
	int retCode = 0;
	
	/* pres */
	ASSERT (dobj != NULL);
	ASSERT (dobj->kind == DISPOBJ_TEXT);
	
	
	msgFile = new QMessageBox (0);
	/*! implementamos la politica de que si no se puede crear => retornamos
	 * false */
	if (msgFile == NULL) {
		debugp ("DispObjFilter::acceptTextObj: Error creando "
		"msgFile\n");
		return false;
	}
	/* creamos los botones */
	msgFile->setStandardButtons (QMessageBox::Ok | QMessageBox::Cancel);
	/* ahora vamos a setear la informacion del archivo */
	msgFile->setText (dobj->getData());
	/* seteamos una micro descripcion */
	msgFile->setInformativeText ("Quiere que se muestre esto "
					"por pantalla?");
	/* mostramos la pantalla */
	retCode = msgFile->exec();
	
	if (retCode == QMessageBox::Ok) {
		/* aceptaron el archivo */
		result = true;
	} else {  /* retCode == QMessageBox::Cancel */
		/* cualquier otro caso NO se acepto el archivo */
		result = false;
	}
	
	/* limpiamos un poco */
	delete msgFile;
	
	return result;
}


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
bool DispObjFilter::acceptPictureObj (const DispObject * dobj)
{	
	QPixmap * pmap = NULL;
	QMessageBox *picWin = NULL;
	bool result = false;
	int retCode = 0;
	
	ASSERT (dobj != NULL);
	ASSERT (dobj->kind == DISPOBJ_PICTURE);
	
	
	picWin = new QMessageBox (0);
	/* la movemos arriba de todos */
	picWin->move (0,0);
	/* obtenemos el pixmap */
	pmap = new QPixmap (dobj->file.fileName());
	
	if (pmap == NULL) {
		/*! erroroncho */
		debugp ("DispObjFilter::acceptPictureObj: error al crear el "
			"pixmap\n");
		/* devolvemos false */
		delete picWin;
		return false;
	}
	/* chequeamos si el tamaño es mayor al soportado */
	if ((pmap->width() >= DOF_MAX_SIZE_X) || 
		(pmap->height () >= DOF_MAX_SIZE_Y)) {
		/* debemos reescalar */
		(*pmap) = pmap->scaled (DOF_MAX_SIZE_X, DOF_MAX_SIZE_Y);
	}
	
	/* la cargamos correctamente => la metemos en la ventana */
	picWin->setIconPixmap ((*pmap));
	/* creamos los botones */
	picWin->setStandardButtons (QMessageBox::Ok | QMessageBox::Cancel);
	/* seteamos una micro descripcion */
	picWin->setInformativeText ("Quiere que se muestre esto "
	"por pantalla?");
	/* mostramos la pantalla */
	retCode = picWin->exec();
	
	if (retCode == QMessageBox::Ok) {
		/* aceptaron el archivo */
		result = true;
	} else {  /* retCode == QMessageBox::Cancel */
		/* cualquier otro caso NO se acepto el archivo */
		result = false;
	}
	
	/* limpiamos */
	delete pmap;
	delete picWin;
	
	return result;
}


/* funcion que guarda un mensaje rechazado en un archivo (REJECTED_O_FN)
* REQUIRES:
*	dobj != NULL 
*/
void DispObjFilter::saveRejectedObj(DispObject *dobj)
{
	fstream filestr;
	QString result = "";
	const CUser *user = NULL;
	char *strDate = NULL;
	time_t nowraw;	
	
	assert(dobj != NULL);
	
	/*! FIXME: solo estamos guardando mensajes de texto por ahora, probablemente
	 * en un futuro se quiera tambien guardar las fotos que mandan "cochinas"
	 */
	if(dobj->kind != DISPOBJ_TEXT)
		return;
	
	result.append("<rejected_msg>");
	
	/* obtenemos la fecha */
	time(&nowraw);
	strDate = ctime(&nowraw);
	if (strDate != NULL){
		result.append(strDate);
		result.remove("\n");
		result.append(": ");
	}
	
	
	user = dobj->getUser();
	if (user != NULL) {
		/* agregamos el usuario */
		result.append("nick: ");
		result.append(user->getNick());
		result.append(", MAC: ");
		result.append(user->getMAC());
		result.append(", ");
	}
	
	result.append("Mensaje: ");
	result.append(dobj->getData());
	result.append("</rejected_msg>");
	
	/* ahora abrimos el arcivo y hacemos un append */
	filestr.open(REJECTED_O_FN, fstream::out | fstream::app);
	filestr << result.toStdString().c_str();
	filestr.close();
}



/* Constructor: Requiere de una base de datos donde pueda chequear
* si existen o no usuarios en un momento determinado.
* REQUIRES:
*	udb != NULL
*/
DispObjFilter::DispObjFilter(UDataBase * db)
{
	ASSERT (db != NULL);
	
	if (db == NULL) {
		debugp ("DispObjFilter::DispObjFilter: db NULL\n");
		return;
	}
	this->udb = db;
}

/* Constructor que no chequea la existencia del usuario en la base
* de datos. Solo muestra al operador si acepta o no el obj 
*/
DispObjFilter::DispObjFilter()
{
	/* vamos a setear la base de datos nula para saber que no hay que
	 * verificarla */
	this->udb = NULL;
}

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
bool DispObjFilter::accept(DispObject * dobj)
{
	bool result = false;
	const CUser * user = NULL;
	
	/* pre */
	ASSERT (dobj != NULL);
	
	/* debemos chequear la base de datos si y solo si la udb != NULL */
	if(this->udb != NULL){
		/* primero vamos a chequear que exista el usuario en la bd */
		user = dobj->getUser();
		if (user == NULL || !(this->udb->existUser (user))) { 
			/*! es null o no existe en la base de datos, de cualquier modo
			* lo borramos al choripan y ni lo mostramos */
			delete dobj;
			return false;
		}
	}
	
	/* ahora vamos a mostrar al usuario dependiendo el tipo de archivo que
	 * sea */
	switch (dobj->kind) {
		case DISPOBJ_TEXT:
			result = acceptTextObj (dobj);
			break;
		case DISPOBJ_PICTURE:
			result = acceptPictureObj (dobj);
			break;
		default:
			/*! zatan? */
			ASSERT (false);
	}
	/* si no se acepto => borramos el archivo y eliminamos el dobj */
	if (!result) {
		debugp2 ("DispObjFilter::accept: El usuario cancelo un archivo"
			"\n");
		/*! guardamos el objeto rechazado! */
		saveRejectedObj(dobj);
		/* borramos el archivo correspondiente */
		if (!(dobj->file.remove ()))
			debugp ("DispObjFilter::accept: Error al intentar"
			" eliminar el archivo\n");
		/* liberamos memoria */
		delete dobj;
	}
	
	return result;
}

/* Destructor:*/
DispObjFilter::~DispObjFilter()
{
	/* do absolutely nothing */
	return;
}
