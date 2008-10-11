#include "textwindow.h"

TextWindow::TextWindow(QWidget *parent)
 : ShowWindow(parent)
{
	text1 = new QTextEdit (0);
	layout = new QVBoxLayout;
	
	/*seteamos el scrollbar vertical*/
	text1->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);
	
	layout->addWidget (text1);
	
	this->setLayout (layout);
	
}

void TextWindow::setMesg (const QString& nombre, const QString& mensaje)
{
	QString aux("");
	aux.append(nombre);
	aux.append( ": ");
	aux.append( mensaje);
	text1->append (aux);
	
}


TextWindow::~TextWindow()
{
}


