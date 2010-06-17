#ifndef MAINPROG_H
#define MAINPROG_H


#include <string>
#include <QString>
#include <QThread>
#include <QObject>

#include "consts.h"
#include "debug.h"
#include "mainwidget.h"
#include "dispobjfilter.h"
#include "udatabase.h"
#include "dispobject.h"
/* bluetooth-sub-system */
#include "btsystemmanager.h"


class MainProg : public QThread
{
	Q_OBJECT
	
	public:
		/* Constructor, no hace falta nada */
		MainProg();
		
		/* Funcion que crea la base de datos.
		 * REQUIRES:
		 * 	fname
		 * 	load (true == load from file, false == new db)
		 * RETURNS:
		 * 	< 0	on error
		 * 	0	if success
		 */
		int createUDB(QString &fname, bool load);
		
		/* Funcion que crea la interfaz grafica 
		* REQUIRES:
		* 	configFile
		* 	callBefor createUDB
		* RETURNS:
		* 	< 0	on error
		* 	0	if success
		*/
		int createGUI(QString &conFile);
		
		/* Funcion que crea el subsistema de recepcion de bluetooth
		 * y lo inicializa y comienza a esperar conexiones.
		 * REQUIRES:
		 * 	load (true => load codes from file, false => newone)
		 * RETURNS:
		 * 	-1	error at creation
		 * 	-2	error at initialization
		 * 	0	if success
		 */
		int creatBTSubSystem(bool load = false);
		
		/* Funcion que ejecuta en un thread aparte (esta misma clase)
		 * todo el sistema completo. 
		 * REQUIRES;
		 * 	UDB	created
		 * 	GUI	created
		 * 	BTSS	created && initialized
		 * RETURNS:
		 * 	< 0	on error
		 * 	0	if success
		 */
		int startSystem(void);
		void run(void);
		
		/* Funcion que detiene la corrida del sistema (para el thread)
		 * y ademas deberia fijarse si guardamos los elementos que
		 * quedaron en la cola del btSM, la lista de codigos usada y
		 * los usuarios registrados...
		 * REQUIRES:
		 * 	startSystem() called
		 * 	save => guarda las cosas
		 */
		void stopSystem(bool save);
		
		/* Libera la memoria de todas las estructuras */
		~MainProg();
	
		
	signals:
		void newObjectReceived(DispObject *obj);
		
	private:
		UDataBase *udb;
		MainWidget *mw;
		BTSystemManager *btSM;
		bool running;
		
		
};

#endif