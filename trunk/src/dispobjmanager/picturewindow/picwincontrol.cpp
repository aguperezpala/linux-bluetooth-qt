#include "picwincontrol.h"





/* Funcion que guarda todas las variables de configuracion */
void PicWinControl::saveConfigs(void)
{
	return;
}


/* Funcion que Carga todas las variables de configuracion */
void PicWinControl::loadConfigs(void)
{
	return;
}

/* Constructor: vamos a pasarle un nombre de archivo que vamos
* a usar para leer las configuraciones y la ventana donde vamos a
* mostrar las fotos.
* REQUIRES:
*	picWin 		!= NULL
* NOTE: puede ser que el archivo de configuracion sea nulo =>
*	 no cargamos ni chori :)
*/
PicWinControl::PicWinControl(QWidget *parent, QString &fname, 
			      PictureWindow *picWin) : QWidget(parent)
{
	this->pw = NULL;
	this->configName = fname;
	/* PRES */
	if (picWin == NULL) {
		ASSERT (false);
		this->close();
		return;
	}
	this->pw = picWin;
	/* cargamos el archivo */
	setupUi(this);
	//QMetaObject::connectSlotsByName(this); /*conectamos cada componente*/
	/*! Cargamos ahora las configuraciones */
	loadConfigs();
}

void PicWinControl::on_txtbuttonSetSize_clicked()
{
	bool ok = false;
	int xres = 0; int yres = 0;
	
	xres = txtsizeX->text().toInt (&ok, 10);
	if (ok) {
		yres = txtsizeY->text().toInt (&ok, 10);
		if (ok)
			this->pw->resize (xres, yres);
	}
}
	
void PicWinControl::on_txtbuttonSetPos_clicked()
{
	bool ok = false;
	int xres = 0; int yres = 0;
	
	xres = txtposX->text().toInt (&ok, 10);
	if (ok) {
		yres = txtposY->text().toInt (&ok, 10);
		if (ok)
			this->pw->move (xres, yres);
	}
	
}


void PicWinControl::on_txtbuttonShowHideWindow_clicked()
{
	if (this->pw->isVisible()) {
		/* entonces el boton lo vamos a cambiar a "Mostrar", ya que la
		 * estamos por esconder */
		txtbuttonShowHideWindow->setText(QString ("Mostrar"));
		this->pw->setVisible (false);
	} else {
		/* al verre */
		txtbuttonShowHideWindow->setText(QString ("Esconder"));
		this->pw->setVisible (true);
	}
}


void PicWinControl::on_txtbuttonSetVelocity_clicked()
{
	bool ok = false;
	int sleepTime = 0;
	
	sleepTime = txttextVelocity->text().toInt(&ok, 10);
	if (ok)
		this->pw->setSleepTime (sleepTime);
}

void PicWinControl::on_txtbuttonStartStop_clicked()
{
	if (this->pw->isPaused()) {
		/* entonces el boton lo vamos a cambiar a "Parar", ya que la
		* estamos por comenzar */
		txtbuttonStartStop->setText(QString ("Parar"));
		this->pw->pause (false);
	} else {
		/* al verre */
		txtbuttonStartStop->setText(QString ("Comenzar"));
		this->pw->pause (true);
	}

}
void PicWinControl::on_txtbuttonStyle_clicked()
{
	Qt::WindowFlags flags = this->pw->windowFlags();
	QPoint pos = this->pw->pos();
	QRect geometry = this->pw->geometry();
	
	if (flags & Qt::FramelessWindowHint)
		/* entonces esta sin bordes => */
		flags = Qt::Widget;
	else
		flags = Qt::FramelessWindowHint;
		
	this->pw->setWindowFlags (flags);
	this->pw->showNormal();
	this->pw->move(pos);
	this->pw->setGeometry (geometry);
}


/* Con esto nos aseguramos de guardar las configuraciones cuando cerramos
 * la ventana.
 */
void PicWinControl::closeEvent(QCloseEvent *event)
{
	
	saveConfigs();
	event->accept();	
}

PicWinControl::~PicWinControl()
{
	/*! Aca escribimos las variables y los valores que queremos escribir
	 * por medio del modulo ConfigManipulator :D
	 */
	
	/*!### <<<Guardamos las cosas en el archivo de configuracion >>> ###*/
	saveConfigs();
	/* nada mas... */
}
