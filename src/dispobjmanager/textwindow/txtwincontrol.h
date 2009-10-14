/*! Esta clase es la encargada de manejar las configuraciones de la ventana de
 * texto (la que muestra la marquesina).
 */

#ifndef TXTWINCONTROL_H
#define TXTWINCONTROL_H

#include <QWidget>
#include <QtGui>
#include <QUiLoader>
#include <QFontDialog>
#include <QFont>


/* manipulacion de archivos */
#include <iostream>
#include <fstream>


#include "../../configmanipulator/configmanipulator.h"
#include "ui_txtwincontrol.h"
#include "textwindow.h"


#define MAINTXTW_CONFIG_FILE_NAME	"config.conf"

class TxtWinControl : public QWidget, private Ui::TxtWinControl 
{
	Q_OBJECT

public:
	/* Constructor: vamos a pasarle un nombre de archivo que vamos
	 * a usar para leer las configuraciones y la ventana de la 
	 * marquesina.
	 * REQUIRES:
	 *	txtWin 		!= NULL
	 * NOTE: puede ser que el archivo de configuracion sea nulo =>
	 *	 no cargamos ni chori :)
	 */
	TxtWinControl(QWidget *parent, QString &fname,
		       TextWindow *txtWin);
	
	/* Funcion que sirve para guardar las configuraciones actuales.
	 * REQUIRES:
	 *	txtWin no haya sido destruida
	 */
	void saveConfigs (void);
	
	
	/* Destructor:
	 * Antes de destruirnos vamos a guardar todas las configuraciones que
	 * tenemos hasta este momento.
	 * NOTE: NO libera el txtWin
	 */
	~TxtWinControl();
	
	

public slots:
	void on_txtbuttonSetSize_clicked();
	void on_txtbuttonSetPos_clicked();
	void on_txtbuttonShowHideWindow_clicked();
	void on_txtbuttonShowWindow_clicked();
	void on_txtbuttonStartStop_clicked();
	void on_txtbuttonStyle_clicked();
	void on_txtbuttonSetFont_clicked();
	void on_txtbuttonSetBackColor_clicked();
	void on_txtbuttonSetFontColor_clicked();
	void on_txtbuttonSetBetween_clicked();
	void on_scrollVel1_valueChanged(int);
	void on_scrollVel2_valueChanged(int);
	void closeEvent (QCloseEvent *);

	
private:
	QString configName;		/* archivo de configuracion */
	TextWindow *tw;		/* ventana donde vamos a mostrar los sms */
	
	
	/* Funciones auxiliares para cargar las configs */
	void loadConfigs (void);
	
};

#endif
