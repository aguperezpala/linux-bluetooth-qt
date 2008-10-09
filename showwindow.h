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

    void setWindowFlags(showWflags flags);
inline void setShowWindowSize (int width, int heigth){setFixedSize(width,heigth);};


private:
    QTextEdit *textEdit;
    QPushButton *closeButton;
};
//! [0]

#endif
