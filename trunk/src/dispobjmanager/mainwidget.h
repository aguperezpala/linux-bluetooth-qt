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


#include "../consts.h"
#include "../debug.h"
#include "picturewindow/picturewindow.h"
#include "picturewindow/picwincontrol.h"
#include "textwindow/textwindow.h"
#include "textwindow/txtwincontrol.h"
#include "../dispobject/dispobject.h"
#include "dispobjtable/dispobjtable.h"
#include "../configmanipulator/configmanipulator.h"
#include "../cuser/cuser.h"


/*! tamaño de la tabla minimo */
#define MW_TABLE_MIN_SIZE_X		600
#define MW_TABLE_MIN_SIZE_Y		400


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
	MainWidget(QWidget *parent = 0, QString & fname);
	
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
	void on_btnDelSelected_clicked ();
	void on_btnShowPic_clicked ();
	void on_btnShowTxt_clicked ();
	void on_btnDelSelected_clicked ();
	void on_btnExit_clicked();
	void closeEvent (QCloseEvent *);

private:
	PictureWindow * pw;		/* ventana de fotos */
	PicWinControl * pwc;		/* ventana de control de fotos */
	TextWindow * tx;		/* ventana de texto */
	TxtWinControl * twc;		/* ventana de control de texto */
	DispObjTable * table;		/* tabla para el operador de DispObjs */
	bool realExit;			/* determinar si es el verdadero exit */
	QString confFile;		/* donde almacenamos el nombre del 
					 * confFile */
		
	
	/* Funciones auxiliares para cargar las configs */
	void saveConfigs (void);
	void loadConfigs (void);
	
	/*! Aca van las funciones que trabajan con la lista de DispObjects 
	 *  NOTE: Recordar que debemos borrar el archivo y el DispObject
	 */
	/* Funcion que entrega un mensaje para mostrar por pantalla. Esta
	 * es para la ventana de textos.
	 * RETURNS:
	 *	str	!= NULL	si es que existe algun mensaje
	 *	NULL	caso contrario.
	 * ENSURES:
	 *	en caso de enviar un mensaje este deja de existir en la tabla.
	 * NOTE: str ya no nos pertenece y debemos borrar ademas el archivo.
	 * NOTE 2: Aca agregamos tambien el nombre del usuario (nickname)
	 *	   antes de mandar los datos.
	 */
	QString * sendMsgSignal (void);
	
	
	/* Funcion que entrega una imagen para mostrar por pantalla. Esta
	* es para la ventana de pictures.
	* RETURNS:
	*	pic	!= NULL	si es que existe alguna imagen
	*	NULL	caso contrario.
	* ENSURES:
	*	una vez mandado una picture, no se encuentra mas en la tabla
	* NOTE: pic ya no nos pertenece y debemos borrar ademas el archivo.
	*/
	QPixmap * sendPicSignal (void);
	
};

#endif
