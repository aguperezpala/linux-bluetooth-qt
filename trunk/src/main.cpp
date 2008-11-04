#include <QApplication>
#include <QMessageBox>
#include <pthread.h>		/*para manejar los threads*/
#include <assert.h>

#include "controllerwindow.h"
#include "mainwidget.h"
#include "textwindow/textwindow.h"
#include "textwindow/maintxtwindow.h"
#include "net/userserver.h"


/*esta funcion va a recibir un UserServer object*/
void *start_user_server (void * s)
{
	UserServer *server = (UserServer*) s;
	
	assert (server != NULL);
	
	/*empezamos a escuchar*/
	server->start();
	
	return NULL;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	MainTxtWindow mtw(0);
	UserServer server(mtw.getUserList());
	pthread_t tserver;		/*thread para el server*/
	int result = 0;
	
	/*******************		USERSERVER		***************/
	server.startListen();
	/*creamos los threads*/
	if (pthread_create (&tserver, NULL, &start_user_server,(void*) &server) != 0){
		printf ("Main: error al crear el thread tserver\n");
	}
	/*********************************************************/
	
	/********************	MAIN PROGRAM	******************/
	mtw.show();
	result = app.exec();
	/*********************************************************/
	
	server.closeServer();
	
	/*esperamos los threads*/
	if (pthread_join (tserver, NULL) != 0)
		printf ("Main: Se termino incorrectamente tserver\n");
	else
		printf ("Main: Se termino correctamente tserver\n");

	
	return result;
}
