#include "textwindow.h"


/*esta funcion modifica el texto que se esta mostrando en pantalla*/
void TextWindow::update_text ()
{
	/*primero vamos a agregar una letra del str*/
	if (this->str.size() == 0){
		if (this->strcount >= MAX_STR_BUFF) {
			this->timer->stop();
			this->text->clear();
		} else {
			this->text->insertPlainText (QString (" "));
			this->strcount++;
		}
	} else {
		this->text->insertPlainText(QString (this->str[0]));
		this->str.remove (0,1); /*sacamos el primer caracter*/
	}
	this->text->moveCursor (QTextCursor::End);
	this->text->ensureCursorVisible();
}
	

TextWindow::TextWindow(QWidget *parent)
 : ShowWindow(parent)
{
	this->timer = NULL;
	this->text = NULL;
	this->strcount = 0;
	
	text = new QTextEdit (0);
	if (this->text == NULL){
		dprintf ("No se puede inicializar el QTextEdit\n");
		this->close(); /*error, no se puede inicializar el timer*/
	}
	
	
	
	layout = new QVBoxLayout;
	
	/*creamos el timer*/
	this->timer = new QBasicTimer();
	
	if (this->timer == NULL){
		dprintf ("No se puede inicializar el timer\n");
		this->close(); /*error, no se puede inicializar el timer*/
	}
	
	
	layout->addWidget (text);
	
	this->setLayout (layout);
	
	/***************configuraciones principales****************/
	this->vel = 100;	
	this->text->setAlignment (Qt::AlignRight);
	this->text->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	this->text->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	this->text->setLineWrapMode (QTextEdit::NoWrap); /*1 renglon*/
	this->text->setSizePolicy (QSizePolicy::Ignored,QSizePolicy::Ignored);
	
	
}

void TextWindow::setMesg (const QString& mensaje)
{
	QString aux = mensaje;
	/*copiamos el string sobre el cual vamos a trabajar y le sacamos los
	 *espacios en blanco*/
	aux.replace( "\n", " ");
	str.append(aux);
	
	this->strcount = 0;
	/*if (!this->timer->isActive()) {
		this->timer->start (this->vel, this);
	}*/
	
}

 void TextWindow::paintEvent(QPaintEvent * /* event */)
{
	/*QFontMetrics metrics(this->text->font());
	int x = (this->text->width() - metrics.width(this->text->text())) / 2;
	int y = (this->text->height() + metrics.ascent() - metrics.descent()) / 2;
	QPainter painter(this);
	painter.setPen(this->color);
	for (int i = 0; i < this->text->size(); ++i) {
		painter.drawText (x + vel, y, QString (this->text[i]));
		x += metrics.width(this->text->toPlainText()[i]);
	}*/
}

 void TextWindow::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == this->timer->timerId()) {
		/*if (str.isEmpty()){
			this->timer->stop(); 
		} else {*/
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
	delete this->text; /*aca es donde vamos a ir mostrando el texto*/
	delete this->layout;
	delete this->timer;
	
}


