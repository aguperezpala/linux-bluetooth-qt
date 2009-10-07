/*! Este modulo se encarga de manejar las configuraciones. Hay que determinar
 * algunos casos especiales:
 * Si el archivo de configuracion no existe => se crea.
 * Si el archivo existe y se intenta escribir una variable ya existente
 * entonces se reemplaza el valor.
 * Si no existe dicha variable => se crea una nueva variable.
 * La forma en que se van a guardar las cosas va a ser:
 * NOMBRE_VAR=VALOR\n
 */
#ifndef CONFIG_MANIPULATOR_H
#define CONFIG_MANIPULATOR_H


#include <QHash>
#include <QString>
#include <QFile>
#include <QByteArray>

#include "../consts.h"
#include "../debug.h"

/* definimos el tamaño maximo que puede tener una linea (linea = var=value\n) */
#define CONFM_MAX_LINE_SIZE		500
#define CONFM_SEPARATOR			'='

class ConfigManipulator {
	public:
		/* Constructor al cual se le debe pasar el nombre del archivo
		 * de configuracion.
		 * NOTE: si el archivo no existe lo crea.
		 */
		ConfigManipulator (QString & fname);
		
		/* Funcion que devuelve el valor de una variable (var).
		 * en caso de que var no exista => devuelve NULL.
		 * RETURNS:
		 *	value  	!= NULL (NOTE: alloca memoria)
		 *	NULL	si var no exista.
		 */
		QString * getValue (QString & var);
		
		/* Funcion que setea un valor (value) en una variable (var)
		 * dentro del archivo de configuracion.
		 * En caso de que var no exista => es creada una nueva entrada.
		 * REQUIRES:
		 *	var.isNull() == false
		 *	value.isNull() == false
		 */
		void setValue (QString & var, QString value);
		
		/* Funcion que guarda los cambios */
		void saveChanges(void);
		
		/*! DEBUG */
		void printConfVars(void);
		
		/* Destructor. Guarda todos los cambios en el archivo */
		~ConfigManipulator();
		
	private:
		QFile file;			/* archivo de conf */
		QHash<QString, QString> hash;	/* almacenamos los datos */
		
};

#endif