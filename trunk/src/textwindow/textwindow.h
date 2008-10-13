#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H

#include <QWidget>
#include <QLayout>
#include <QTextEdit>
#include "../showwindow.h"
#include <QBasicTimer>
#include "../debug.h"

class TextWindow : public ShowWindow
{
public:
    	TextWindow(QWidget *parent);
    
    /*en esta funcion vamos a mostrar por pantalla el mensaje*/
    	void setMesg (const QString& nombre, const QString& mensaje);

inline	void setVelocity (int v){vel = v;};
	
	void setFontType (QString& font_name, int size, QFont::Weight type);

    ~TextWindow();
protected:
	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event);

private:
	QTextEdit *text; /*aca es donde vamos a ir mostrando el texto*/
	QLayout *layout;
	int vel;
	QBasicTimer *timer;
};

#endif
