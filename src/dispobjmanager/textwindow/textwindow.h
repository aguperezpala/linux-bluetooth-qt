/*! Esta clase vamos a pedir una DispObjTable para ir sacando los elementos de
 * ahi, vendria a ser una especie de "repositorio" de datos.
 * El funcionamiento general va a ser: Se van obteniendo QStrings para mostrar,
 * cada vez que se nescesite (osea cuando haya espacio para mostrar otro mensaje
 * se intenta pedir un nuevo QString, si se obtiene se muestra, si no no).
 * Cada objeto a mostrar en pantalla sera un MarquesinObj.
 * NOTE: Es un QWidget.
*/

#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H

#include <QWidget>
#include <QThread>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QFontMetrics>
#include <QColor>
#include <QPainter>
#include <QFont>
#include <QList>
#include <QCloseEvent>

#include "../../consts.h"
#include "../../debug.h"
#include "marquesinobj.h"
#include "../dispobjtable/dispobjtable.h"

#define MAX_STR_BUFF	800	/*cantidad de caracteres que pueden estar en
pantalla*/



class TextWindow : public QWidget
{
	Q_OBJECT
			
public:
	/* Constructor: Muestra y crea la nueva ventana.
	 * REQUIRES:
	 *	doTable != NULL ("repositorio de datos")
	 * Necesitamos que el parent tenga una funcion del tipo que requerimos
	 * NOTE: tener en cuenta que debe devolver NULL si no hay siguiente
	 * mensaje. ### Esta funcion se encarga de liberar el QString, NO debe
	 * ser liberado desde otro lado.
	*/
	TextWindow(DispObjTable * doTable);
	
    
	/* Funcion que agrega un un mensaje a la cola. Ademas saca los '\n' para
	* serializar los datos en una misma linea y agrega el espacio "betwen"
	* al final de cada mensaje.
	* REQUIRES:
	*		msj.isNull () == false
	*/    
	void setMesg (const QString& msj);
    	
	
	/* Funcion que pausa (p = true) o continua (p = false) el movimiento
	 * de las letras.
	 */
	void pause(bool p);
	
	/* funcion que devuelve si estamos corriendo o no los msjs */
	bool isPaused(void);
	
	void setVelocity (int v);		/* Refresh time ms */
	void setStep (int s) {this->step = s;};	/* step size */	
	void setBetween (QString& b) {this->between = b;};
	
	/* setea el color de la fuente */
	void setFontColor (QColor & c) { this->color = c; };
	inline QColor & getFontColor (void){return this->color;};
	/* setea una nueva fuente para el texto */
	void setTextFont (QFont & font);
	
	/* setea el color de backgorund */
	void setBackColor (const QColor& c);
	
	~TextWindow();
	
public slots:
	void closeEvent (QCloseEvent *);
	
protected:
	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event);
	

private:
	
	/* Funcion que entrega un mensaje para mostrar por pantalla.
	Esta
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
	QString * getNextMsg (void);
	
	/* funcion actualiza las metrics cuando se cambio el tipo de fuente*/
	void setNewMetricsFont(void);
	
	/* Funcion que hace practicamente todo, actualiza las posiciones,
	 * chequea si se debe buscar un nuevo mensaje para mostrar, agregarlo
	 * etc.
	 * NOTE: esta es la funcion principal
	 */
	void updateText(void);
	
	/*agrega un mensaje para mostrar en pantalla
	RETURNS:
	true == si se pudo agregar
	false == cc
	*/
	
	/* Funcion que agrega un mensaje a la "cola" mlist para ser mostrado
	 * en pantalla.
	 * RETURNS;
	 *	true 	if success
	 *	false	otherwise
	 */
	bool addMesg(void);
	
	
	bool canWakeUp;		/*para determinar si debemos despertar o no al text*/
	int vel;		/*refresh time*/
	int step;		/*step size*/
	
	QBasicTimer timer;	/* nuestro "thread */
	QFontMetrics *metrics;	/* para determinar el tamaño de la fuente */
	QPainter painter;	/* el que dibuja sobre el QWidget */
	QString between;	/* string entre medio de cada mensaje */
	QColor color;		/* es el color de la fuente */
	
	QList<MarquesinObj *> mlist;	/*marquesin list,*/
	
	/*! funcion que obtiene el proximo elemento */
	DispObjTable * table;
};

#endif
