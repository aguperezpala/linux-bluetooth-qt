#include <QApplication>
#include <QMessageBox>
#include <pthread.h>		/*para manejar los threads*/
#include <assert.h>

#include "controllerwindow.h"
#include "mainwidget.h"
#include "textwindow/textwindow.h"
#include "textwindow/maintxtwindow.h"
#include "net/userserver.h"
#include "reader/smsreader.h"

#define FIFO_FILE_NAME "fifo"	/*nombre del archivo fifo*/

/*esta funcion va a recibir un UserServer object*/
void *start_user_server (void * s)
{
	UserServer *server = (UserServer*) s;
	
	assert (server != NULL);
	
	/*empezamos a escuchar*/
	server->start();
	printf ("Main: Se termino de escuchar con server\n");
	return NULL;
}


/*funcion que inicializa el reader
	REQUIRES:
			s != NULL (s == smsreader)

*/
void *start_sms_reader (void * s)
{
	SmsReader *reader = (SmsReader*) s;
	
	assert (reader != NULL);
	if (!reader->setFile (FIFO_FILE_NAME)){
		printf ("Main: No existe el archivo %s (SmsReader no se puede"
				" inicializar)\n", FIFO_FILE_NAME);
		exit (1);
	} else
		printf ("Main: SmsReader inicializado correctamente.\n");
	/*empezamos a escuchar*/
	reader->startReading();
	printf ("Main: Se termino de leer con reader\n");
	return NULL;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	MainTxtWindow mtw(0);
	UserServer server(mtw.getUserList());
	SmsReader reader(&mtw);
	pthread_t tserver;		/*thread para el server*/
	pthread_t treader;		/*thread para el reader*/
	int result = 0;
	
	/*******************		USERSERVER		***************/
	if (!server.startListen()) {
		printf ("Main: imposible de escuchar en esos puertos. Server no"
					" inicializado.\n");
		exit (1);
	} else
		printf ("Main: Server inicializado correctamente.\n");
	/*creamos los threads*/
	if (pthread_create (&tserver, NULL, &start_user_server,(void*) &server) != 0){
		printf ("Main: error al crear el thread tserver\n");
	}
	/*********************************************************/
	
	
	/*******************		SMSREADER		***************/
	/*creamos los threads*/
	if (pthread_create (&treader, NULL, &start_sms_reader,(void*) &reader) != 0){
		printf ("Main: error al crear el thread treaderr\n");
	}
	/*********************************************************/
	
	
	/********************	MAIN PROGRAM	******************/
	
	mtw.show();
	result = app.exec();
	/*********************************************************/
	
	server.closeServer();
	reader.stopReading();
	/*esperamos los threads*/
	if (pthread_join (tserver, NULL) != 0)
		printf ("Main: Se termino incorrectamente tserver\n");
	else
		printf ("Main: Se termino correctamente tserver\n");
	
	if (pthread_join (treader, NULL) != 0)
		printf ("Main: Se termino incorrectamente treader\n");
	else
		printf ("Main: Se termino correctamente treader\n");

	
	return result;
}
