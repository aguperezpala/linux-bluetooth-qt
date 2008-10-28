/*!Lo que va hacer esta clase es:

*/

#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H

#include <QWidget>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QColor>
#include <QPainter>
#include <QFont>
#include <QList>

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
	
	
	/*esta funcion la llamamos cuando tenemos un nuevo mensaje en la SmsTable*/
	void signalNewMesg();

	void setVelocity (int v);	/*Refresh time ms*/
	void setStep (int s) {this->step = s;};	/*step size*/
	
	void setBetween (QString& b) {this->between = b;};
	/*void setFontType (QString& font_name, int size, QFont::Weight type);
	void setFontSize (int size);*/
inline	void setTextFont (QFont& f){this->setFont(f);};
	
inline	const QFont& getTextFont (){return this->font();};
	
	void start(){if (!timer->isActive())timer->start(this->vel,this); canWakeUp = true;};	/*comienzan a moverse las letras*/
	void stop(){timer->stop(); canWakeUp = false;};	/*frena el movimiento de las letras*/
	
inline	void setBackColor (const QColor& c){/*this->setTextBackgroundColor (c);*/};
inline	void setFontColor (const QColor& c){this->color = c;};
		
	

    ~TextWindow();
protected:
	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event);

private:
	void update_text();
	bool add_sms();
	
	bool canWakeUp;		/*para determinar si debemos despertar o no al text*/
	int vel;			/*refresh time*/
	int step;			/*step size*/
	int pos;
	int firstptr;			/*variable auxiliar para almacenar la pos inicial del str*/
	int lastptr;			/*posicion final del string (seria el principio de la prox*/
	QBasicTimer *timer;
	QString str;		/*Donde vamos a mostrar el mensaje*/
	QString between;	/*string entre medio de cada mensaje*/
	int strcount;
	SmsTable *smsTable;
	QColor color;
	QFontMetrics *metrics;
	
};

#endif
