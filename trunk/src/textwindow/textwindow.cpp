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
	
	
	/*Chequeamos que haya elementos en la tabla*/
	if (!this->smsTable->isEmpty()) {
		dprintf ("estamos agregando sms\n");
		/*si no es vacia sacamos el sms de la tabla/cola*/
		sms = this->smsTable->popFront();
		if (sms != NULL) {
			/*lo metemos para mostrar*/
			this->setMesg (*(sms->getMesg()));
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
	MarquesinObj *mobj = NULL;
	QString *data = NULL;
	int size = 0;
	
	/*debemos chequear que el primer marquesinobj este dentro del rango visible
	y a su vez que si la "cola" del mismo ya esta siendo vista, entonces debemos
	agregar otro marquesinobj*/
	if (!this->mlist.isEmpty()) {
		mobj = this->mlist.last();	/*el ultimo tomamos*/
		if (mobj != NULL) {
			data = mobj->getData();
			if (data != NULL) {
				/*chequeamos si debemos agregar otro sms*/
				size = this->metrics->width(*data);
				if (size < (this->width() - mobj->getShowPos())) {
					/*debemos agregar otro*/
					add_sms();
				}
			} /*else no hacemos nada*/
		}
		/*ahora chequeamos si debemos borrar alguno*/
		mobj = NULL;
		data = NULL;
		
		mobj = this->mlist.first();	/*el primero tomamos*/
		if (mobj != NULL) {
			data = mobj->getData();
			if (data != NULL) {
				/*vemos si lo debemos sacar o no de la lista*/
				size = this->metrics->width(*data);
				if ((this->width() - mobj->getShowPos() + size) < 0) {
					/*debemos sacarlo*/
					delete data;
					delete mobj; /*liberamos memoria*/
					this->mlist.removeFirst();	/*lo sacamos de la lista*/
					dprintf ("eliminamos uno de mlist\n");
				}
			} /*else no hacemos nada*/
		}
		
	} else {
		/*no hay nada para hacer.. frenamos el timer*/
		//this->timer->stop();
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
	this->step = 10;
	this->between = ".....   ";
	this->canWakeUp = true;
	
}

void TextWindow::setMesg (const QString& mensaje)
{
	MarquesinObj *mobj = NULL;
	QString aux = mensaje;
	QString *straux = NULL;
	/*copiamos el string sobre el cual vamos a trabajar y le sacamos los
	 *espacios en blanco*/
	aux.replace( "\n", " ");
	
	straux = new QString (aux);
	if (straux != NULL){
		straux->append (this->between);
		
		/*creamos el marquesinobj en la posicion final*/
		mobj = new MarquesinObj (straux, 0);
		if (mobj != NULL) {
			/*la agregamos a la lista*/
			this->mlist.append (mobj);
			dprintf ("se agrego a mlist: size = %d \n", this->mlist.size());
		} else {
			delete straux;
		}
	}
	
	if (!this->timer->isActive()) {
		this->timer->start (this->vel, this);
	}
}


void TextWindow::signalNewMesg()
{
	/*chequeamos que si esta activo el timer, entonces quiere decir que esta
	 funcionando*/
	if (this->mlist.isEmpty())
		add_sms();
	if (!this->timer->isActive() && canWakeUp) {
		/*agregamos un sms de la smstable*/
	
		dprintf ("signal\n");
		/*activamos el timer*/
		this->timer->start (this->vel, this);
	}
}
		


 void TextWindow::paintEvent(QPaintEvent * /* event */)
{	
	int x = this->width();
	int y = (this->height() + metrics->ascent() - metrics->descent()) / 2;
	int delta = 0;
	MarquesinObj *mobj = NULL;
	QString *str = NULL;
	
	QPainter painter(this);
	painter.setPen(color);
		
	for (int j = 0; j < mlist.size(); j++){
		/*vamos a recorrer la cantidad de MarquesinObj que tengamos*/
		mobj = NULL;
		x = this->width();
		str = NULL;
		
		mobj = this->mlist.at(j);	/*obtenemos el j MarquesinObj*/
		if (mobj != NULL) {
			str = mobj->getData();
			for (int i = 0; i < str->size(); ++i) {
				delta = x - mobj->getShowPos();
				if (delta > 0) {
					/*si estamos dentro del tamaño de la pantalla dibujamos*/
					painter.drawText(delta, y, QString((*str)[i]));
					x += metrics->width((*str)[i]);
				} else {
					/*no tenemos que dibujar mas.. salimos del ciclo*/
					break;
				}
			}
		}
	}
}

 void TextWindow::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == this->timer->timerId()) {
		MarquesinObj *mobj;
		/*if (str.isEmpty()){
			this->timer->stop(); 
		} else {*/
		/*debemos actualizar las posiciones de cada MarquesinObj*/
		for (int i = 0; i < this->mlist.size(); i++) {
			mobj = NULL;
			mobj = this->mlist.at(i);
			if (mobj != NULL) 
				mobj->setShowPos ((mobj->getShowPos() + this->step));
		}
		
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


