#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H

#include <QWidget>
#include <QLayout>
#include <QTextEdit>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QColor>
#include <QPainter>

#include "../showwindow.h"
#include "../debug.h"



class TextWindow : public ShowWindow
{
public:
    	TextWindow(QWidget *parent);
    
    /*en esta funcion vamos a mostrar por pantalla el mensaje*/
	/*!Tener en cuenta que tenemos que mandarle la cadena de caracteres
	que va a separar un mensaje de otro... */
    	void setMesg (const QString& mensaje);

	void setVelocity (int v);
	
	void setFontType (QString& font_name, int size, QFont::Weight type);
	void setFontSize (int size);
	void setColor (int red, int green, int blue);	/*default negro*/
	
	void start(){timer->start(60,this);};	/*comienzan a moverse las letras*/
	void stop(){timer->stop();};	/*frena el movimiento de las letras*/
	

    ~TextWindow();
protected:
	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event);

private:
	void update_text();
	QTextEdit *text; /*aca es donde vamos a ir mostrando el texto*/
	QLayout *layout;
	int vel;
	QBasicTimer *timer;
	QColor color;	/*color del texto*/
	QString str;
	
};

#endif
