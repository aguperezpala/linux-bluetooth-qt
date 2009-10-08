#include "picturewindow.h"



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
	this->picture = this->getNextPic();
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
*	getNextPic != NULL 
* NOTE: tener en cuenta que debe devolver NULL si no hay siguiente
* 	 imagen. 
* ### Esta funcion se encarga de liberar el QPixmap, NO debe ser 
*     liberado desde otro lado.
*/
PictureWindow::PictureWindow(QPixmap * (*getNextPic1)(void))
{
	QPalette p;
	
	/* pres */
	if (getNextPic1 == NULL) {
		ASSERT (false);
		debugp ("Error getNextPic == NULL");
		this->close();
	}
	this->paused = false;
	this->getNextPic = getNextPic1;
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



