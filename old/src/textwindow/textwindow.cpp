#include "textwindow.h"


/* Funcion que agrega un mensaje a la "cola" mlist para ser mostrado
* en pantalla.
* RETURNS;
*	true 	if success
*	false	otherwise
*/
bool TextWindow::addMesg()
{
	QString *msj = NULL;
	bool result = false;
	
	/*! ASSERT (this->getNextMsg != NULL); siempre */
	
	/* pedimos un msj para agregar */
	msj = this->getNextMsg();
	
	/* si tenemos un nuevo msj => creamos un MarquesinObj y lo metemos
	 * en la lista.
	 */
	if (msj != NULL) {
		/* si hay */
		result = true;
		/* lo encolamos */
		setMesg(*msj);
		/* borramos el msj */
		delete msj; msj = NULL;
	}
	
	
	return result;
}


/* Funcion que hace practicamente todo, actualiza las posiciones,
* chequea si se debe buscar un nuevo mensaje para mostrar, agregarlo
* etc.
* NOTE: esta es la funcion principal
*/
void TextWindow::updateText ()
{
	MarquesinObj *mobj = NULL;
	const QString *data = NULL;
	int size = 0;
	
	/* verificamos que la lista no este vacia (si no no deberiamos haber
	 * llamado a esta funcion...? */
	if (this->mlist.isEmpty()) {
		/* verificamos de sacar un nuevo elemento (getNextMsg) */
		if (addMesg() == false) {
			/* no podemos agregar nada, frenamos el timer y 
			 * volvemos ya que no hay nada que hacer */
			this->timer.stop();
			return;
		}
		/* si no es porque se agrego un nuevo mensaje y debemos
		 * trabajarlo */
	}
	
	/* actualizamos las posiciones de los objetos */
	
	for (int i = 0; i < this->mlist.size(); i++) {
		mobj = NULL;
		mobj = this->mlist.at(i);
		if (mobj != NULL) 
			mobj->setShowPos ((mobj->getShowPos() - this->step));
	}
	
	
	/*! aca ya sabemos que por lo menos tenemos un elemento */
	mobj = this->mlist.last();
	ASSERT (mobj != NULL); /*! es imposible */
	data = mobj->getData();
	ASSERT (data != NULL); /*! de nuevo, es imposible por marquesinobj */
	
	/* chequeamos si debemos agregar otro sms */
	if (!mobj->addSomeOne) {
		size = this->metrics->width(*data);
		if (mobj->getShowPos() + size < this->width() ) {
			/* El final del objeto esta ya adentro de la pantalla,
			 * lo que indica que ya podemos agregar otro elemento
			*/
			mobj->addSomeOne = addMesg();
		}
	}
		
	/* ahora chequeamos si debemos borrar alguno */
	mobj = NULL;
	data = NULL;
	
	/* ahora tomamos el primero, que seria el que estaria por terminar 
	 * primero */
	mobj = this->mlist.first();
	ASSERT (mobj != NULL); /*! es imposible */
	data = mobj->getData();
	ASSERT (data != NULL); /*! de nuevo, es imposible por marquesinobj */
	
	/* vemos si lo debemos sacar o no de la lista */
	size = this->metrics->width(*data);
	if (mobj->getShowPos() + size < 0) {
		/* debemos sacarlo */
		delete mobj; /*liberamos memoria*/
		mobj = NULL;
		this->mlist.removeFirst();	/*lo sacamos de la lista*/
		debugp2 ("Eliminamos uno de mlist\n");
	}
}


/* Constructor: Muestra y crea la nueva ventana.
* REQUIRES:
*	getNextMsg != NULL (funcion q obtiene el proximo sms a mostrar)
* NOTE: tener en cuenta que debe devolver NULL si no hay siguiente
* mensaje. ### Esta funcion se encarga de liberar el QString, NO debe
* ser liberado desde otro lado.
*/
TextWindow::TextWindow(QString * (*getNextMsg1)(void))
{
	/* pres */
	ASSERT (getNextMsg1 != NULL);
	
	if (getNextMsg1 == NULL) {
		debugp ("Error getNextMsg == NULL");
		this->close();
	}
	this->getNextMsg = getNextMsg1;
	
	this->metrics = new QFontMetrics(this->font());
	ASSERT (this->metrics != NULL);
	
	
	this->vel = 100;
	this->step = 10;
	this->between = "    ";
	
	
}


/* Funcion que agrega un un mensaje a la cola. Ademas saca los '\n' para
* serializar los datos en una misma linea y agrega el espacio "betwen"
* al final de cada mensaje.
* REQUIRES:
*		msj.isNull () == false
*/    
void TextWindow::setMesg (const QString& msj)
{
	MarquesinObj *mobj = NULL;
	QString *straux = NULL;
	
	
	ASSERT (msj.isNull() == false);
	
	if (msj.isNull()) {
		debugp ("Error msj.isNull");
		return;
	}
	/* Creamos el string */
	straux = new QString (msj);
	
	if (straux == NULL)
		return;
	
	/* limpiamos los \n */
	straux->replace( "\n", " ");
	straux->append (this->between);
	
	/* creamos el marquesinobj en la posicion final */
	mobj = new MarquesinObj (straux, this->width());
	if (mobj != NULL) {
		/*la agregamos a la lista*/
		this->mlist.append (mobj);
	} else {
		delete straux;
	}
	
	
	if (!this->timer.isActive()) {
		this->timer.start (this->vel, this);
	}
}

/* Esta funcion es llamada cada vez que se hace un QWidget->update();.
 * Refresca toda la pantalla
 */
void TextWindow::paintEvent(QPaintEvent * /* event */)
{	
	MarquesinObj *mobj = NULL;
	const QString *str = NULL;
	int x = 0, j = 0, i = 0;
	int y = (this->height() + this->metrics->ascent() -
			this->metrics->descent()) / 2;
	
	this->painter.begin (this);
	/* vamos a recorrer la cantidad de MarquesinObj que tengamos */
	this->painter.setPen(this->color);
	for (j = 0; j < mlist.size(); j++){
		mobj = NULL;
		str = NULL;
		
		/* obtenemos el j MarquesinObj */
		mobj = this->mlist.at(j); 
		
		/* si es null NO DEBERIA, lo salteamos */
		if (mobj == NULL)
			continue;
		
		x = mobj->getShowPos();
		str = mobj->getData();
		
		for (i = 0; i < str->size(); ++i) {
			if (x <= this->width()) {
				/* si estamos dentro del tamaño de la pantalla
				 * dibujamos */
				this->painter.drawText(x, y,
							QString((*str)[i]));
				/* aumentamos la posicion de la proxima letra */
				x += metrics->width((*str)[i]);
			} else 
				/* nada que dibujar.. salimos del ciclo */
				break;
		}
	}
	this->painter.end();
}

/* Esta funcion es llamada en cada timeout() event del timer. */
void TextWindow::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == this->timer.timerId()) {
		/* debemos actualizar las posiciones de cada MarquesinObj */
		updateText();
		/* ahora refrescamos el Widget e imprimimos todo de nuevo 
		 * osea llama a paintEvent
		 */
		update();
	} else {
		QWidget::timerEvent(event);
	}
	
}

void TextWindow::pause(bool p) 
{
	if (p) {
		this->timer.stop();
	} else
		if (!this->timer.isActive())
			this->timer.start(this->vel, this);
}

bool TextWindow::isPaused(void)
{
	return !this->timer.isActive();
}

void TextWindow::setNewMetricsFont()
{
	if (this->metrics != NULL)
		delete this->metrics;
	this->metrics = NULL;
	this->metrics = new QFontMetrics(this->font());
}

void TextWindow::setTextFont (QFont & font)
{
	this->setFont (font);
	setNewMetricsFont();
}


void TextWindow::setBackColor (const QColor& c)
{
	QPalette p;
	
	p.setColor (QPalette::Window, c);
	this->setPalette (p);
}


void TextWindow::setVelocity (int v)
{
	this->vel = v;
	debugp2 ("TextWindow::setVelocity: setearon la velocidad\n");
	/* lo frenamos y le seteamos el nuevo interval */
	if (!this->timer.isActive ())
		this->timer.stop ();
	/* lo prendemos con el nuevo interval */
	this->timer.start (v, this);
}

TextWindow::~TextWindow()
{
	QList<MarquesinObj*>::iterator i;
	MarquesinObj * mobj = NULL;
	
	/* debemos liberar todos los marquesin object */
	for (i = this->mlist.begin(); i != this->mlist.end(); ++i) {
		mobj = (MarquesinObj*) *i;
		if (mobj != NULL) {
			delete mobj; mobj = NULL;
		}
	}
	
}



