#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H

#include <QWidget>
#include <QLayout>
#include <QTextEdit>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QColor>
#include <QPainter>
#include <QFont>
#include "../showwindow.h"
#include "../debug.h"

#define MAX_STR_BUFF	800	/*cantidad de caracteres que pueden estar en pantalla*/

class TextWindow : public ShowWindow
{
	Q_OBJECT
			
public:
    	TextWindow(QWidget *parent);
    
    /*en esta funcion vamos a mostrar por pantalla el mensaje*/
	/*!Tener en cuenta que tenemos que mandarle la cadena de caracteres
	que va a separar un mensaje de otro... */
    	void setMesg (const QString& mensaje);

	void setVelocity (int v);
	
	/*void setFontType (QString& font_name, int size, QFont::Weight type);
	void setFontSize (int size);*/
inline	void setTextFont (QFont& f){this->text->setFont(f);};
	
inline	const QFont& getTextFont (){return this->text->font();};
	
	void start(){if (!timer->isActive())timer->start(this->vel,this);};	/*comienzan a moverse las letras*/
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
	QString str;
	int strcount;
	
};

#endif
