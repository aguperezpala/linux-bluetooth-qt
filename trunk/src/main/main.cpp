#include <stdio.h>
#include <string>
#include <iostream>
#include <QApplication>
#include <QString>

#include "mainprog.h"
using namespace std;



int main (int argc, char ** argv)
{	
	MainProg mainProg;
	QApplication app(argc, argv, true);
	QString fname = "";

	cout << "Comenzando el sistema\n";
	
	/*! FIXME: deberiamos ver aca si tenemos que cargar de archivo 
	 * algo o no */
	
	
	/* creamos la base de datos */
	fname = "users.udb";
	if (mainProg.createUDB(fname, false) < 0) {
		cout << "Error al intentar crear la base de datos en el Mainprog\n";
		exit(1);
	}
	cout << "Base de datos creada\n";
	/* creamos la interfaz grafica */
	fname = "config.conf";
	if(mainProg.createGUI(fname) < 0){
		cout << "Error al crear la GUI\n";
		exit(1);
	}
	cout << "GUI creada\n";
	/* creamos el subsistema de BT */
	if(mainProg.creatBTSubSystem(false) < 0){
		cout << "Error al crear el subsistema de BT\n";
		exit(1);
	}
	cout << "BT SubSystem creado\n";
	/* corremos el server */
	if(mainProg.startSystem() < 0) {
		cout << "Error al intentar correr el sistema \n";
		exit(1);
	}
	cout << "Corriendo el sistema\n";
	app.exec();
	/*! aca deberiamos setear el tema de guardar las configuraciones capaz */
	cout << "Terminando de correr el sistema!\n";
	return 0;
}