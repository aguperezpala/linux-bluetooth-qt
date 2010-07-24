#include <stdio.h>
#include <time.h>
#include <string>
#include <iostream>
#include <QApplication>
#include <QString>
#include <QMessageBox>

#include "mainprog.h"
using namespace std;

/*! ### definimos el nombre del archivo de la bd de datos y archivo de 
 * configuracion */
#define BD_FILE_NAME		"users.udb"
#define CONFIG_FILE_NAME	"config.conf"
/* ahora vamos a definir el nombre del archivo donde vamos a guardar la fecha
 * de la ultima ejecucion */
#define DATE_FILE_NAME		"have_load.conf"
/* el archivo de codigos es BTRECEIVER_CODES_FILE */


/* Funcion que elimina los archivos de usuarios y de codigos */
static void remove_uc_files(void)
{
	remove(BD_FILE_NAME);
	remove(BTRECEIVER_CODES_FILE);
}

/*! Esta funcion va a verificar si tenemos que cargar o no a los codigos
 * y a los usuarios, fijandonos en la fecha.
 * Funcionamiento: 1º vamos a leer un archivo (have_load.conf) y verificar
 * la fecha guardada en el mismo. Si la misma es del mismo dia o un dia anterior
 * entonces se abrira una nueva ventana preguntando si se desea cargar o no
 * los usuarios ya registrados. En caso de que la fecha sea de mayor de 1 dia
 * no se cargara ningun usuario nuevo y seran eliminados todos aquellos
 * codigos y usuarios registrados en los archivos correspondientes.
 * 
 * RETURNS:
 * 	true	si se tiene que cargar los datos
 * 	false 	si no se tienen que cargar los datos
 */
static bool have_to_load_users(void)
{
	fstream filestr;
	time_t yestraw, nowraw;	
	struct tm *yesttm = NULL;
	struct tm *nowtm = NULL;
	int nm, nd, ym, yd, fd, fm;
	bool result = false;
	QMessageBox *loadBox = NULL;
	int retCode = 0;
	char *strDate = NULL;
	QString msg = "";
	
	
	
	/* ahora leemos la fecha actual */
	time(&nowraw);
	
	yesttm = new tm;
	nowtm = new tm;
	
	if (yesttm == NULL || nowtm == NULL)
		return false;
	
	yestraw = nowraw-60*60*24;
	memcpy(nowtm,localtime(&nowraw),sizeof(struct tm));
	memcpy(yesttm,localtime(&yestraw),sizeof(struct tm));
	nm = (int) nowtm->tm_mon;
	nd = (int) nowtm->tm_mday;
	ym = (int) yesttm->tm_mon;
	yd = (int) yesttm->tm_mday;
	
	delete yesttm;
	delete nowtm;
	
	filestr.open (DATE_FILE_NAME, fstream::in | fstream::out | fstream::binary);
	if (!(filestr.good() && filestr.is_open())){
		/* no esta creado, asique creamos uno nuevo y volvemos */
		cout << "Creando un nuevo archivo " << DATE_FILE_NAME << endl;
		filestr.close();
		filestr.open (DATE_FILE_NAME, fstream::out | fstream::binary);
		filestr << nm << " " << nd;
		filestr.close();
		return false;
	}
	
	/* si estamos aca es porque si existe el archivo => leemos la fecha */
	filestr >> fm >> fd;
	if (filestr.fail()){
		cout << "Error al leer " << DATE_FILE_NAME << endl;
		filestr.close();
		return false;
	}
	
	/* ahora verificamos si la fecha leida es la actual o la de ayer */
	if (!((fm == nm && fd == nd) || (fm == ym && fd == yd))){
		/* no es ni de hoy ni de ayer => sobrescribimos la fecha y 
		 * devolvemos false */
		remove_uc_files();
		filestr.write ((char *)nowtm, sizeof(struct tm));
		filestr.close();
		return false;
	}
	filestr.close();
	/* si estamos aca es porque es la correcta, preguntamos entonces si
	 * queremos cargar los archivos */
	loadBox = new QMessageBox (0);
	
	if (loadBox == NULL) {
		cout << "Error al abrir la ventana de pregunta de carga de usuarios\n";
		return false;
	}
	/* creamos los botones */
	loadBox->setStandardButtons (QMessageBox::Ok | QMessageBox::Cancel);
	msg = "¿Desea cargar los usuarios recientemente registrados en la fecha ";
	strDate = ctime(&nowraw);
	if (strDate != NULL){
		msg.append(strDate);
	}
	msg.append(" ?");
	loadBox->setText (msg);
	/* mostramos la pantalla */
	retCode = loadBox->exec();
	if (retCode == QMessageBox::Ok) {
		/* aceptaron el archivo */
		result = true;
	} else {  /* retCode == QMessageBox::Cancel */
		/* cualquier otro caso NO se acepto el archivo */
		result = false;
	}
	
	/* limpiamos un poco */
	delete loadBox;
	
	return result;
}


int main (int argc, char ** argv)
{	
	QApplication app(argc, argv, true);
	MainProg mainProg;
	QString fname = "";
	bool haveToLoad = false;

	cout << "Comenzando el sistema\n";
	
	haveToLoad = have_to_load_users();
	
	/* creamos la base de datos */
	fname = BD_FILE_NAME;
	if (mainProg.createUDB(fname, haveToLoad) < 0) {
		cout << "Error al intentar crear la base de datos en el Mainprog\n";
		exit(1);
	}
	cout << "Base de datos creada\n";
	/* creamos la interfaz grafica */
	fname = CONFIG_FILE_NAME;
	if(mainProg.createGUI(fname) < 0){
		cout << "Error al crear la GUI\n";
		exit(1);
	}
	cout << "GUI creada\n";
	/* creamos el subsistema de BT */
	if(mainProg.creatBTSubSystem(haveToLoad) < 0){
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
	
	cout << "Terminando de correr el sistema!\n";
	
	return 0;
}