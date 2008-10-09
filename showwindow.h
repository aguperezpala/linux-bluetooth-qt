/* Vamos a usar esta clase para abrir cualquier tipo de ventana, ya sea para 
 * mostrar fotos o lo que sea... vamos a implementarlas desde otras instancias
 */
#ifndef SHOWWINDOW_H
#define SHOWWINDOW_H

#include <QWidget>


/*vamos a hacer un enum para determinar el tipo de ventana*/
typedef enum {
	SW_WITH_BORDERS,
	SW_WITHOUT_BORDERS
} showWflags;

QT_BEGIN_NAMESPACE
class QPushButton;
class QTextEdit;
QT_END_NAMESPACE

//! [0]
class ShowWindow : public QWidget
{
    Q_OBJECT

public:
    ShowWindow(QWidget *parent = 0);
    /*para setear el tipo de ventana*/	
    void setWindowFlags(showWflags flags);
    
    /*seteamos el tamaño de la ventana*/
inline    void setWindowSize (int width, int heigth){setFixedSize(width,heigth);};
    
    /*destructor*/
    
    


private:
};
//! [0]

#endif
