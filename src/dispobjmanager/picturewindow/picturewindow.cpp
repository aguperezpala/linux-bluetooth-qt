#include "picturewindow.h"




/* Funcion que entrega una imagen para mostrar por pantalla. Esta
* es para la ventana de pictures.
* RETURNS:
*	pic	!= NULL	si es que existe alguna imagen
*	NULL	caso contrario.
* ENSURES:
*	una vez mandado una picture, no se encuentra mas en la tabla
* NOTE: pic ya no nos pertenece y debemos borrar ademas el archivo.
*/
QPixmap * PictureWindow::getNextPic (void)
{
	DispObject * obj = NULL;
	QPixmap * result = NULL;
	
	/* sacamos de pecho el primer elemento que corresponda al tipo
	* DISPOBJ_PICTURE */
	obj = this->table->popFirst (DISPOBJ_PICTURE);
	
	/* si es null => devolvemos NULL */
	if (obj == NULL)
		return NULL;
	
	/* ahora en teoria tenemos un objeto, lo vamos a cargar en la imagen */
	result = new QPixmap ();
	/* lo cargamos */
	if (result->load (obj->file.fileName()) == false) {
		/*! no pudimos cargarlo ni bosta */
		debugp ("MainWidget::sendPicSignal: no se pudo cargar la foto "
		"algun error interno :S... o que lo que\n");
		/* limpiamos la basura */
		if (result)
			delete result;
		delete obj; /* no nos sirve mas */
		return NULL; /* no vamos a poderla mostrar :( */
	}
	/* lo pudimos cargar correctamente :) => eliminamos el archivo ya que
	* no nos hace mas falta, y ademas devolvemos la imagen */
	delete obj;
	
	return result;
}




/* Esta funcion hace atomica la escritura de la variable newPicture
* para evitar inconsistencias 
*/
void PictureWindow::setNewPictureFlag (bool b)
{
	this->mutex.lock();
	this->newPicture = b;
	this->mutex.unlock();
}

/* Esta funcion hace atomica la lectura de la variable newPicture
* para evitar inconsistencias 
*/
bool PictureWindow::getNewPictureFlag (void)
{
	bool result = false;
	
	this->mutex.lock();
	result = this->newPicture;
	this->mutex.unlock();
	
	return result;
}


/* Esta es la funcion que va a manejar el tema de mostrar/borrar/pedir
* nuevas imagenes cuando sea necesario.
* Es la funcion llamanda desde el timer.
*/
void PictureWindow::updatePicture(void)
{
	/*! vamos a eliminar la imagen actual si es que hay alguna */
	this->label.clear();
	if (this->picture) {
		/* eliminamos la imagen */
		delete this->picture; this->picture = NULL;
	}
	
	/* intentamos pedir una imagen */
	this->picture = getNextPic();
	/* no hay mas imagen que mostrar... aca debemos determinar
	* una politica determinada, si no mostramos mas fotos,
	* o mostramos la ultima foto.... o mostramos alguna por
	* defecto. Por el momento vamos a mostrar ninguna (para
	* que cualquier cosa se muestre el video).
	*/
	if (this->picture) {
		/* si tenemos una imagen => la mostramos por pantalla */
		this->label.setPixmap ((*this->picture));
		/* ajustamos la imagen al tamaño de la ventana */
		this->label.adjustSize();
	}
	
	
}


/* Constructor: Muestra y crea la nueva ventana.
* REQUIRES:
*	doTable != NULL 
* NOTE: tener en cuenta que debe devolver NULL si no hay siguiente
* 	 imagen. 
* ### Esta funcion se encarga de liberar el QPixmap, NO debe ser 
*     liberado desde otro lado.
*/
PictureWindow::PictureWindow(DispObjTable * doTable)
{
	QPalette p;
	
	/* pres */
	if (doTable == NULL) {
		ASSERT (false);
		debugp ("PictureWindow::PictureWindow: Error doTable == NULL");
		this->close();
	}
	this->paused = false;
	this->table = doTable;
	this->newPicture = false;
	this->sleepTime = 7000;	/* 7 segundos por foto */
	this->picture = NULL;
	this->layout.addWidget(&(this->label));
	this->setLayout (&this->layout);
	
	/* configuramos el label para que se ajuste al tamaño */
	this->label.setScaledContents(true);
	this->label.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	
	/*! color negro de fondo... */
	p.setColor (QPalette::Window, QColor::QColor(0,0,0));
	this->setPalette (p);
}


/* Funcion que avisa a la ventana que llego una nueva imagen.
* Lo que hace es despertar el timer en caso de que este apagado,
* si se estan mostrando actualmente imagenes => no hace nada.
*/    
void PictureWindow::setPicture (void)
{
	/* la hacemos atomica */
	setNewPictureFlag (true);
	/* activamos el timer si esta frenado... pero no pausado */
	if (!this->paused)
		pause(false);
}

/* Esta funcion es llamada en cada timeout() event del timer. */
void PictureWindow::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == this->timer.timerId()) {
		/* nos llamaron con el timer. */
		updatePicture();
	} else {
		QWidget::timerEvent(event);
	}
	
}

void PictureWindow::pause(bool p) 
{
	this->paused = p;
	if (p) 
		this->timer.stop();
	else
		if (!this->timer.isActive())
			this->timer.start(this->sleepTime, this);
		
}

bool PictureWindow::isPaused(void)
{
	return !this->timer.isActive();
}

/* funcion que setea el sleepTime (tiempo a mostrar cada foto)
* REQUIRES:
*	ms >= 10
*/
void PictureWindow::setSleepTime (int ms)
{
	this->sleepTime = ms;
	/* lo frenamos y le seteamos el nuevo interval */
	if (!this->timer.isActive ())
		this->timer.stop ();
	/* lo prendemos con el nuevo interval */
	this->timer.start (ms, this);
}

void PictureWindow::closeEvent (QCloseEvent *event)
{	
	if (false)
		event->accept();
	else
		event->ignore();
	
}

PictureWindow::~PictureWindow()
{
	if (this->picture)
		delete this->picture;
}



