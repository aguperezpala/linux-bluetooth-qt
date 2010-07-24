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
#include <QString>
#include <QCloseEvent>
#include <QMutex>
#include <QByteArray>

#include "ui_mainwidget.h"
#include "../consts.h"
#include "../debug.h"
#include "../dispobject/dispobject.h"
#include "dispobjtable/dispobjtable.h"
#include "picturewindow/picturewindow.h"
#include "picturewindow/picwincontrol.h"
#include "textwindow/textwindow.h"
#include "textwindow/txtwincontrol.h"
#include "../configmanipulator/configmanipulator.h"
#include "../cuser/cuser.h"
#include "dispobjfilter.h"



class MainWidget : public QWidget, private Ui::MainWidget
{
    Q_OBJECT

public:
	/* Constructor: Le vamos a pasar el archivo de configuracion
	 * que tiene que leer.
	 * REQUIRES;
	 *	fname.isNull == false
	 * NOTE: si el archivo no existe, es creado.
	 */
	MainWidget(QWidget *parent, QString & fname);
	
	/* Funcion que agrega un elemento "mostrable" (DispObject).
	 * En teoria ya fue aceptado por el usuario.
	 * REQUIRES:
	 *	dobj	!= NULL
	 * NOTE: Una vez que se pasa el dobj NO debe ser eliminado el mismo.
	 *	 pasa a ser propiedad de esta clase.
	 */
	void addDispObject (DispObject * dobj);
	
	/* Destructor, esto destruye ### TODAS ### las ventanas, tener
	 * cuidado con esto.
	 */
	~MainWidget();


	
public slots:
	void acceptNewObject(DispObject *dobj);
	void on_btnDelSelected_clicked ();
	void on_btnExit_clicked();
	void closeEvent (QCloseEvent *);

private:
	PictureWindow * pw;		/* ventana de fotos */
	PicWinControl * pwc;		/* ventana de control de fotos */
	TextWindow * tw;		/* ventana de texto */
	TxtWinControl * twc;		/* ventana de control de texto */
	DispObjTable * table;		/* tabla para el operador de DispObjs */
	bool realExit;			/* determinar si es el verdadero exit */
	QString confFile;		/* donde almacenamos el nombre del 
					 * confFile */
	DispObjFilter *dof;
		
	
	/* Funciones auxiliares para cargar las configs */
	void saveConfigs (void);
	void loadConfigs (void);
	/* Funcion que destruye todo el contenido de esta clase */
	void destroyAll (void);
	
	
};

#endif
