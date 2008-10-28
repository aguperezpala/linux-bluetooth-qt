/*!Lo que va hacer esta clase es:
 * Cada vez que tenemos un sms para mostrar (de la SmsTable), lo extraemos, lo 
 * ponemos para mostrar en la marquesina (y ahi nomas encolamos el "between". 
 * Se muestra y una vez que se termina de mostrar lo que hacemos es pasar el 
 * mensaje "between" (espacios entre mensajes). Una vez terminado todo lo 
 * desencolamos y encolamos el proximo.
*/

#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H

#include <QWidget>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QColor>
#include <QPainter>
#include <QFont>

#include "../showwindow.h"
#include "../debug.h"
#include "smstable.h"
#include "smsobject.h"

#define MAX_STR_BUFF	800	/*cantidad de caracteres que pueden estar en pantalla*/

class TextWindow : public ShowWindow
{
	Q_OBJECT
			
public:
	/*El constructor requiere que le pasemos como argumento la tabla de donde va
	 *extraer los elementos para mostrar en la "marquesina"
		REQUIRES:
				table != NULL
	*/
    	TextWindow(QWidget *parent, SmsTable *table);
    
    /*en esta funcion vamos a mostrar por pantalla el mensaje y agrega el 
	 * between al final del mensaje (espaciador entre mesajes)
	 *saca los \n para "serializarlos en una misma linea*/
    void setMesg (const QString& mensaje);

	void setVelocity (int v);	/*Refresh time ms*/
	void setStep (int s) {this->step = s;};	/*step size*/
	
	void setBetween (QString& b) {this->between = b;};
	/*void setFontType (QString& font_name, int size, QFont::Weight type);
	void setFontSize (int size);*/
inline	void setTextFont (QFont& f){this->setFont(f);};
	
inline	const QFont& getTextFont (){return this->font();};
	
	void start(){if (!timer->isActive())timer->start(this->vel,this);};	/*comienzan a moverse las letras*/
	void stop(){timer->stop();};	/*frena el movimiento de las letras*/
	
inline	void setBackColor (const QColor& c){/*this->setTextBackgroundColor (c);*/};
inline	void setFontColor (const QColor& c){this->color = c;};
		
	

    ~TextWindow();
protected:
	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event);

private:
	void update_text();
	int vel;			/*refresh time*/
	int step;			/*step size*/
	int pos;			/*variable auxiliar para almacenar la pos del str*/
	QBasicTimer *timer;
	QString str;		/*Donde vamos a mostrar el mensaje*/
	QString between;	/*string entre medio de cada mensaje*/
	int strcount;
	SmsTable *smsTable;
	QColor color;
	QFontMetrics *metrics;
	
};

#endif
