#include "textwindow.h"
#include <QFont>

TextWindow::TextWindow(QWidget *parent)
 : ShowWindow(parent)
{
	this->timer = NULL;
	this->text = NULL;
	
	
	text = new QTextEdit (0);
	if (this->text == NULL){
		dprintf ("No se puede inicializar el QTextEdit\n");
		this->close(); /*error, no se puede inicializar el timer*/
	}
	/*seteamos el scrollbar vertical*/
	text->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);
	
	
	layout = new QVBoxLayout;
	
	/*creamos el timer*/
	this->timer = new QBasicTimer();
	
	if (this->timer == NULL){
		dprintf ("No se puede inicializar el timer\n");
		this->close(); /*error, no se puede inicializar el timer*/
	}
	
	
	layout->addWidget (text);
	
	this->setLayout (layout);
	
}

void TextWindow::setMesg (const QString& nombre, const QString& mensaje)
{
	QString aux("");
	aux.append(nombre);
	aux.append( ": ");
	aux.append( mensaje);
	text->append (aux);
	
}

 void TextWindow::paintEvent(QPaintEvent * /* event */)
{
	/*
	QFontMetrics metrics(font());
	int x = (width() - metrics.width(text)) / 2;
	int y = (height() + metrics.ascent() - metrics.descent()) / 2;
	QColor color;

	QPainter painter(this);
	for (int i = 0; i < text.size(); ++i) {
		int index = (step + i) % 16;
		color.setHsv((15 - index) * 16, 255, 191);
		painter.setPen(color);
		painter.drawText(x, y - ((sineTable[index] * metrics.height()) / 400),
				 QString(text[i]));
		x += metrics.width(text[i]);
	}*/
}

 void TextWindow::timerEvent(QTimerEvent *event)
{/*
	if (event->timerId() == timer.timerId()) {
		++step;
		update();
	} else {
		QWidget::timerEvent(event);
	}*/
}

void TextWindow::setFontType (QString& font_name, int size, QFont::Weight type)
{
	this->text->setFont (QFont (font_name, size,  type));
}


TextWindow::~TextWindow()
{
}


