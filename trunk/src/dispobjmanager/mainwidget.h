/*! Esta es la ventana principal, donde vamos a tener la tabla de objetos
  * recibidos, y a su vez las opciones para controlar las diferentes ventanas
  * de control.
  */

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QtGui>
#include <QtUiTools>


#include "../consts.h"
#include "../debug.h"
#include "picturewindow/picturewindow.h"
#include "picturewindow/picwincontrol.h"
#include "textwindow/textwindow.h"
#include "textwindow/txtwincontrol.h"

class MainWidget : public QWidget, private Ui::MainWidget
{
    Q_OBJECT

public:
	MainWidget(QWidget *parent = 0);
	
public slots:
	void on_button1_clicked ();

private:
	PictureWindow * pw;		/* ventana de fotos */
	PicWinControl * pwc;		/* ventana de control de fotos */
	TextWindow * tx;		/* ventana de texto */
	TxtWinControl * twc;		/* ventana de control de texto */
};

#endif
