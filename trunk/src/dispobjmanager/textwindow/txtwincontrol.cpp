#include "txtwincontrol.h"





/* Funcion que guarda todas las variables de configuracion */
void TxtWinControl::saveConfigs(void)
{
	ConfigManipulator * cm = NULL;
	QString var = "";
	QString value = "";
	
	
	/* verificamos que tw != NULL */
	if (this->tw == NULL) {
		debugp ("TxtWinControl::saveConfigs: tw NULL\n");
		return;
	}
	/* creamos el config manipulator */
	cm = new ConfigManipulator (this->configName);
	
	/* ahora vamos a configurar todo */
	var = "TXT_FONT";
	value = (this->tw->font()).family();
	cm->setValue (var, value);
	
	var = "TXT_FONT_PIXEL_SIZE";
	value.setNum((this->tw->font()).pixelSize());
	cm->setValue (var, value);
	
	var = "TXT_FONT_POINT_SIZE";
	value.setNum((this->tw->font()).pointSize());
	cm->setValue (var, value);
	
	var = "TXT_FONT_COLOR";
	value.setNum (this->tw->getFontColor().rgb());
	cm->setValue (var, value);
	
	var = "TXT_BACK_COLOR";
	value.setNum (this->tw->palette().color (QPalette::Background).rgb());
	cm->setValue (var, value);
	
	var = "TXT_WIN_POSITION_X";
	value.setNum((this->tw->pos()).x());
	cm->setValue (var, value);
	
	var = "TXT_WIN_POSITION_Y";
	value.setNum((this->tw->pos()).y());
	cm->setValue (var, value);
	
	var = "TXT_WIN_SIZE_X";
	value.setNum((this->tw->size()).width());
	cm->setValue (var, value);
	
	var = "TXT_WIN_SIZE_Y";
	value.setNum((this->tw->size()).height());
	cm->setValue (var, value);
	
	var = "TXT_WIN_STYLE";
	value.setNum(this->tw->windowFlags());
	cm->setValue (var, value);
	
	var = "TXT_VELOCITY1";
	value.setNum(this->tw->getVelocity());
	cm->setValue (var, value);
	
	var = "TXT_VELOCITY2";
	value.setNum(this->tw->getStep());
	cm->setValue (var, value);
	
	var = "TXT_BETWEN";
	value = this->tw->getBetween();
	cm->setValue (var, value);
	
	/* guardamos y salimos */
	cm->saveChanges();
	delete cm;
	
}


/* Funcion que Carga todas las variables de configuracion */
void TxtWinControl::loadConfigs(void)
{
	ConfigManipulator * cm = NULL;
	QString var = "";
	QString * value = NULL;
	QColor color;
	bool noError = true;
	int aux = 0;
	uint uaux = 0;
	
	
	/* verificamos que tw != NULL */
	if (this->tw == NULL) {
		debugp ("TxtWinControl::loadConfigs: tw NULL\n");
		return;
	}
	/* creamos el config manipulator */
	cm = new ConfigManipulator (this->configName);
	
	/* cargamos la fuente y todo lo nescesario */
	{
		QString * font = NULL, * pixSize = NULL, * pointSize = NULL;
		int poSize = -1, piSize = -1;
		QFont auxFont;
		
		var = "TXT_FONT";
		font = cm->getValue (var);
		if (font == NULL)
			font = new QString ("");
		var = "TXT_FONT_POINT_SIZE";
		pointSize = cm->getValue (var);
		if (pointSize != NULL) {
			poSize = pointSize->toInt (&noError);
			if (!noError)
				poSize = -1;
			delete pointSize;
		}
		var = "TXT_FONT_PIXEL_SIZE";
		pixSize = cm->getValue (var);
		if (pixSize != NULL) {
			piSize = pixSize->toInt (&noError);
			if (!noError)
				piSize = -1;
			delete pixSize;
		}
		/* ahora cargamos la fuente */
		auxFont = QFont::QFont(*font, poSize, piSize);
		this->tw->setTextFont (auxFont);
		delete font;
	}
	
	var = "TXT_FONT_COLOR";
	value = cm->getValue (var);
	if (value != NULL) {
		uaux = value->toUInt (&noError);
		color = QColor::QColor (uaux);
		this->tw->setFontColor (color);
		delete value; value = NULL;
	}
	
	var = "TXT_BACK_COLOR";
	value = cm->getValue (var);
	if (value != NULL) {
		uaux = value->toUInt (&noError);
		color = QColor::QColor (uaux);
		this->tw->setBackColor (color);
		delete value; value = NULL;
	}
	
	var = "TXT_WIN_POSITION_X";
	value = cm->getValue (var);
	if (value != NULL) {
		aux = value->toInt (&noError);
		this->tw->move (aux, 0);
		delete value; value = NULL;
	}
	
	var = "TXT_WIN_POSITION_Y";
	value = cm->getValue (var);
	if (value != NULL) {
		aux = value->toInt (&noError);
		this->tw->move (this->tw->pos().x(), aux);
		delete value; value = NULL;
	}
	
	var = "TXT_WIN_SIZE_X";
	value = cm->getValue (var);
	if (value != NULL) {
		aux = value->toInt (&noError);
		this->tw->resize (aux, 0);
		delete value; value = NULL;
	}
	
	var = "TXT_WIN_SIZE_Y";
	value = cm->getValue (var);
	if (value != NULL) {
		aux = value->toInt (&noError);
		this->tw->resize (this->tw->size().width(), aux);
		delete value; value = NULL;
	}
	
	var = "TXT_WIN_STYLE";
	value = cm->getValue (var);
	if (value != NULL) {
		Qt::WindowFlags flags = 0;
		flags = (Qt::WindowFlags) value->toUInt (&noError);		
		this->tw->setWindowFlags (flags);
		delete value; value = NULL;
	}
	
	var = "TXT_VELOCITY1";
	value = cm->getValue (var);
	if (value != NULL) {
		aux = value->toInt (&noError);
		this->tw->setVelocity (aux);
		this->scrollVel1->setValue(aux);
		delete value; value = NULL;
	}
	
	var = "TXT_VELOCITY2";
	value = cm->getValue (var);
	if (value != NULL) {
		aux = value->toInt (&noError);
		this->tw->setStep (aux);
		this->scrollVel2->setValue(aux);
		delete value; value = NULL;
	}
	
	var = "TXT_BETWEN";
	value = cm->getValue (var);
	if (value != NULL) {
		this->tw->setBetween (*value);
		delete value; value = NULL;
	}
	
	delete cm;
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
