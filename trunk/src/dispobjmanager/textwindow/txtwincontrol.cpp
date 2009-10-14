#include "txtwincontrol.h"





/* Funcion que guarda todas las variables de configuracion */
void TxtWinControl::saveConfigs(void)
{
	ConfigManipulator * cm = NULL;
	
	/* verificamos que tw != NULL */
	if (this->tw == NULL) {
		debugp ("TxtWinControl::saveConfigs: tw NULL\n");
		return;
	}
	/* creamos el config manipulator */
	cm = new ConfigManipulator (this->configName);
	/* ahora vamos a configurar todo */
	cm->
	
}


/* Funcion que Carga todas las variables de configuracion */
void TxtWinControl::loadConfigs(void)
{
	return;
}

TxtWinControl::TxtWinControl(QWidget *parent, QString &fname, 
	       TextWindow * txtWin) : QWidget(parent)
{
	this->tw = NULL;
	this->configName = fname;
	/* PRES */
	if (txtWin == NULL) {
		ASSERT (false);
		this->close();
		return;
	}
	this->tw = txtWin;
	/* cargamos el archivo */
	setupUi(this);
	//QMetaObject::connectSlotsByName(this); /*conectamos cada componente*/
	/*! Cargamos ahora las configuraciones */
	loadConfigs();
}

void TxtWinControl::on_txtbuttonSetSize_clicked()
{
	bool ok = false;
	int xres = 0; int yres = 0;
	
	xres = txtsizeX->text().toInt (&ok, 10);
	if (ok) {
		yres = txtsizeY->text().toInt (&ok, 10);
		if (ok)
			this->tw->resize (xres, yres);
	}
}
	
void TxtWinControl::on_txtbuttonSetPos_clicked()
{
	bool ok = false;
	int xres = 0; int yres = 0;
	
	xres = txtposX->text().toInt (&ok, 10);
	if (ok) {
		yres = txtposY->text().toInt (&ok, 10);
		if (ok)
			this->tw->move (xres, yres);
	}
	
}

void TxtWinControl::on_txtbuttonSetBetween_clicked()
{
	QString aux(this->txtlineBetween->text());
	this->tw->setBetween(aux);
}
void TxtWinControl::on_txtbuttonShowHideWindow_clicked()
{
	if (this->tw->isVisible()) {
		/* entonces el boton lo vamos a cambiar a "Mostrar", ya que la
		 * estamos por esconder */
		txtbuttonShowHideWindow->setText(QString ("Mostrar"));
		this->tw->setVisible (false);
	} else {
		/* al verre */
		txtbuttonShowHideWindow->setText(QString ("Esconder"));
		this->tw->setVisible (true);
	}
}
void TxtWinControl::on_txtbuttonShowWindow_clicked()
{
	this->tw->setVisible (true);
}


void TxtWinControl::on_txtbuttonStartStop_clicked()
{
	if (this->tw->isPaused()) {
		/* entonces el boton lo vamos a cambiar a "Parar", ya que la
		* estamos por comenzar */
		txtbuttonStartStop->setText(QString ("Parar"));
		this->tw->pause (false);
	} else {
		/* al verre */
		txtbuttonStartStop->setText(QString ("Comenzar"));
		this->tw->pause (true);
	}

}
void TxtWinControl::on_txtbuttonStyle_clicked()
{
	Qt::WindowFlags flags = this->tw->windowFlags();
	QPoint pos = this->tw->pos();
	QRect geometry = this->tw->geometry();
	
	if (flags & Qt::FramelessWindowHint)
		/* entonces esta sin bordes => */
		flags = Qt::Widget;
	else
		flags = Qt::FramelessWindowHint;
		
	this->tw->setWindowFlags (flags);
	this->tw->showNormal();
	this->tw->move(pos);
	this->tw->setGeometry (geometry);
}
void TxtWinControl::on_txtbuttonSetFont_clicked()
{
	bool ok = false;
	QFont font = QFontDialog::getFont(&ok,this->tw->font(), this->tw);
	if (ok) {
		this->tw->setTextFont (font);
	}
}

void TxtWinControl::on_txtbuttonSetBackColor_clicked()
{
	QColor color = QColorDialog::getColor(this->tw->palette().color (
		QPalette::Background), this);
	if (color.isValid()) {
		this->tw->setBackColor (color);
	}
}
void TxtWinControl::on_txtbuttonSetFontColor_clicked()
{
	QColor color = QColorDialog::getColor(this->tw->getFontColor(), this);
	if (color.isValid()) {
		this->tw->setFontColor (color);
	}
}

void TxtWinControl::on_scrollVel1_valueChanged(int v)
{
	this->tw->setVelocity(v);
}
void TxtWinControl::on_scrollVel2_valueChanged(int s)
{
	this->tw->setStep (s);
}

/* Con esto nos aseguramos de guardar las configuraciones cuando cerramos
 * la ventana.
 */
void TxtWinControl::closeEvent(QCloseEvent *event)
{
	/* guardamos la configuracion */
	saveConfigs();
	/*! NO cerramos, simplemente, escondemos... */
	this->setVisible (false);
	event->ignore();
	
}

TxtWinControl::~TxtWinControl()
{
	/*! Aca escribimos las variables y los valores que queremos escribir
	 * por medio del modulo ConfigManipulator :D
	 */
	
	/*!### <<<Guardamos las cosas en el archivo de configuracion >>> ###*/
	saveConfigs();
	/* nada mas... */
}
