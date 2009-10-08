/*! Es la ventana sola con la imagen a mostrar. Esta clase va a tomar una
 * funcion especial del tipo QPixmap * (*getNextPic)(void); que lo que va hacer
 * es obtener el la siguiente imagen a mostrar. Esto nos permite borrar el
 * archivo (ya que en el PixMap tenemos la foto, osea en memoria)
 * El funcionamiento va a ser el siguiente, a cada vencimiento del clock vamos
 * a pedir la siguiente imagen, si hay la mostramos, si no hay => nos dormimos.
 * En caso de que no haya, entonces nos dormimos hasta que nos llaman de nuevo
 * que llego una nueva foto.
 * NOTE: Debemos tener en cuenta de que solo de a 1 foto podamos visualizar, ya
 * 	 que son almacenadas en RAM.
 * NOTE: Es un QWidget.
*/

#ifndef PICTUREWINDOW_H
#define PICTUREWINDOW_H

#include <QWidget>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QCloseEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QMutex>


#include "../../consts.h"
#include "../../debug.h"


class PictureWindow : public QWidget
{
	Q_OBJECT
			
public:
	/* Constructor: Muestra y crea la nueva ventana.
	 * REQUIRES:
	 *	getNextPic != NULL 
	 * NOTE: tener en cuenta que debe devolver NULL si no hay siguiente
	 * 	 imagen. 
	 * ### Esta funcion se encarga de liberar el QPixmap, NO debe ser 
	 *     liberado desde otro lado.
	*/
	PictureWindow(QPixmap * (*getNextPic)(void));
	
    
	/* Funcion que avisa a la ventana que llego una nueva imagen.
	 * Lo que hace es despertar el timer en caso de que este apagado,
	 * si se estan mostrando actualmente imagenes => no hace nada.
	*/    
	void setPicture (void);
    	
	
	/* Funcion que pausa (p = true) o continua (p = false) el movimiento
	 * de las letras.
	 */
	void pause(bool p);
	
	/* funcion que devuelve si estamos corriendo o no los msjs */
	bool isPaused(void);
	
	/* funcion que setea el sleepTime (tiempo a mostrar cada foto)
	 * REQUIRES:
	 *	ms >= 10
	 */
	void setSleepTime (int ms);
	
	/* Destructor */
	~PictureWindow();
	
public slots:
	void closeEvent (QCloseEvent *);
	
protected:	
	void timerEvent(QTimerEvent *event);
	

private:
	/* Esta funcion hace atomica la escritura de la variable newPicture
	 * para evitar inconsistencias 
	 */
	void setNewPictureFlag (bool b);
	
	/* Esta funcion hace atomica la lectura de la variable newPicture
	* para evitar inconsistencias 
	*/
	bool getNewPictureFlag (void);
	
	
	/* Esta es la funcion que va a manejar el tema de mostrar/borrar/pedir
	 * nuevas imagenes cuando sea necesario.
	 * Es la funcion llamanda desde el timer.
	 */
	void updatePicture(void);
	
	
	
	bool paused;		/* determinamos si estamos en pausa */
	QMutex mutex;		/* para hacer atomico el seteo del flag */
	bool newPicture;	/* nos avisa si hay una nueva imagen */
	int sleepTime;		/* Tiempo a mostrar cada foto */
	QBasicTimer timer;	/* nuestro "thread */
	QPixmap * picture;	/* almacenamos el puntero a la ultima pic */
	QVBoxLayout layout;	/* para agregar el label */
	QLabel label;		/* por medio de esto vamos a poder mostrar */
	/*! funcion que obtiene el proximo elemento */
	QPixmap * (*getNextPic)(void);
};

#endif
