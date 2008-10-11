#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H

#include <QWidget>
#include <QLayout>
#include <QTextEdit>
#include "../showwindow.h"

class TextWindow : public ShowWindow
{
public:
    TextWindow(QWidget *parent);
    
    /*en esta funcion vamos a mostrar por pantalla el mensaje*/
    void setMesg (const QString& nombre, const QString& mensaje);
    
    
    ~TextWindow();

private:
	QTextEdit *text1; /*aca es donde vamos a ir mostrando el texto*/
	QLayout *layout;
};

#endif
