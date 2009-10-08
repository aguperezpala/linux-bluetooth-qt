/*! Esta clase es la encargada de manejar las configuraciones de la ventana de
 * imagenes (donde se van a mostrar las fotos).
 */

#ifndef PICWINCONTROL_H
#define PICWINCONTROL_H

#include <QWidget>
#include <QtGui>
#include <QUiLoader>
#include <QFontDialog>
#include <QFont>


/* manipulacion de archivos */
#include <iostream>
#include <fstream>


#include "ui_picwincontrol.h"
#include "picturewindow.h"


#define MAINTXTW_CONFIG_FILE_NAME	"config.conf"

class PicWinControl : public QWidget, private Ui::PicWinControl 
{
	Q_OBJECT

public:
	/* Constructor: vamos a pasarle un nombre de archivo que vamos
	 * a usar para leer las configuraciones y la ventana donde vamos a
	 * mostrar las fotos.
	 * REQUIRES:
	 *	picWin 		!= NULL
	 * NOTE: puede ser que el archivo de configuracion sea nulo =>
	 *	 no cargamos ni chori :)
	 */
	PicWinControl(QWidget *parent, QString &fname,
		       PictureWindow *picWin);
	
	/* Destructor:
	 * Antes de destruirnos vamos a guardar todas las configuraciones que
	 * tenemos hasta este momento.
	 * NOTE: NO libera el picWin
	 */
	~PicWinControl();
	
	

public slots:
	void on_txtbuttonSetSize_clicked();
	void on_txtbuttonSetPos_clicked();
	void on_txtbuttonShowHideWindow_clicked();	
	void on_txtbuttonSetVelocity_clicked();
	void on_txtbuttonStartStop_clicked();
	void on_txtbuttonStyle_clicked();
	void closeEvent (QCloseEvent *);

	
private:
	QString configName;		/* archivo de configuracion */
	PictureWindow *pw;		/* ventana dnd mostramos las fotos */
	
	
	/* Funciones auxiliares para cargar las configs */
	void saveConfigs (void);
	void loadConfigs (void);
	
};

#endif
