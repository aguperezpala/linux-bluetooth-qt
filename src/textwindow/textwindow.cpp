#include "textwindow.h"
#include <assert.h>
/*agrega un mensaje para mostrar en pantalla
	RETURNS:
			true == si se pudo agregar
			false == cc
*/
bool TextWindow::add_sms()
{
	SmsObject *sms = NULL;
	bool result = false;
	
	dprintf ("estamos agregando sms\n");
	/*Chequeamos que haya elementos en la tabla*/
	if (!this->smsTable->isEmpty()) {
		/*si no es vacia sacamos el sms de la tabla/cola*/
		sms = this->smsTable->popFront();
		if (sms != NULL) {
			/*lo metemos para mostrar*/
			this->setMesg (*(sms->getMesg()));
			dprintf ("setMesg\n");
			/*ahora borramos el sms*/
			/*!TENER EN CUENTA SI QUEREMOS AGREGAR NICKNAME*/
			delete sms;
			result = true;
		}
	}
	
	return result;
}

/*esta funcion modifica el texto que se esta mostrando en pantalla*/
void TextWindow::update_text ()
{
	/*debemos distinguir si ya se termino de mostrar uno asi mostramos "between"*/
	if (this->pos - metrics->width(str) > this->width() - metrics->width (between)) {
		
		this->pos = 0;	/*posicionamos de nuevo el puntero a la derecha de todo*/
		if (!add_sms()) {
			/*frenamos el timer*/
			this->timer->stop();
			this->canWakeUp = true;
		}
	}
}
	

TextWindow::TextWindow(QWidget *parent, SmsTable * table)
 : ShowWindow(parent)
{
	this->timer = NULL;
	this->strcount = 0;
	
	/*nos aseguramos que table != NULL*/
	assert (table != NULL);
	this->smsTable = table;
	
	
	
	/*creamos el timer*/
	this->timer = new QBasicTimer();
	
	if (this->timer == NULL){
		dprintf ("No se puede inicializar el timer\n");
		this->close(); /*error, no se puede inicializar el timer*/
	}
	
	
	/***************configuraciones principales****************/
	this->vel = 100;	
	this->metrics = new QFontMetrics(this->font());
	this->step = 0;
	this->pos = 0;
	this->between = ".....   ";
	this->canWakeUp = true;
	this->timer->stop();
}

void TextWindow::setMesg (const QString& mensaje)
{
	QString aux = mensaje;
	/*copiamos el string sobre el cual vamos a trabajar y le sacamos los
	 *espacios en blanco*/
	aux.replace( "\n", " ");
	str.append(aux);
	str.append (this->between);	/*le agregamos el espacio between*/
	
	this->pos = 0;
	
	if (!this->timer->isActive()) {
		this->timer->start (this->vel, this);
	}
	
}


void TextWindow::signalNewMesg()
{
	/*chequeamos que si esta activo el timer, entonces quiere decir que esta
	 funcionando*/
	
	if (!this->timer->isActive() && canWakeUp) {
		/*agregamos un sms de la smstable*/
		add_sms();
		dprintf ("signal\n");
		/*activamos el timer*/
		this->timer->start (this->vel, this);
	}
}
		


 void TextWindow::paintEvent(QPaintEvent * /* event */)
{	
	int x = this->width();
	int y = (this->height() + metrics->ascent() - metrics->descent()) / 2;
	
	QPainter painter(this);
	painter.setPen(color);
	for (int i = 0; i < str.size(); ++i) {
		painter.drawText(x-pos, y, QString(str[i]));
		x += metrics->width(str[i]);
	}
}

 void TextWindow::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == this->timer->timerId()) {
		/*if (str.isEmpty()){
			this->timer->stop(); 
		} else {*/
		this->pos+=this->step;
		update_text();
		update();
	} else {
		QWidget::timerEvent(event);
	}
	
}

/*void TextWindow::setFontType (QString& font_name, int size, QFont::Weight type)
{
	this->text->setFont (QFont (font_name, size,  type));
}

void TextWindow::setFontSize (int size)
{
	this->text->setFont (QFont(this->text->fontFamily(), size, this->text->fontWeight()));
}*/


void TextWindow::setVelocity (int v)
{
	this->vel = v;
	dprintf ("setvelocity\n");
	if (this->timer->isActive())
		this->timer->stop();
	this->timer->start (this->vel,this);
}

TextWindow::~TextWindow()
{
	delete this->timer;
	delete this->metrics;
}


