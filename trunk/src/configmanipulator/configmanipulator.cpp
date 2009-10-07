#include "configmanipulator.h"

/* Constructor al cual se le debe pasar el nombre del archivo
* de configuracion.
* NOTE: si el archivo no existe lo crea.
*/
ConfigManipulator::ConfigManipulator (QString & fname)
{
	QByteArray data;
	QString key = "";
	QString value = "";
	int eqPos = 0;
	
	this->file.setFileName (fname);
	/* lo abrimos para escritura / lectura */
	this->file.open (QIODevice::ReadWrite);
	
	data = this->file.readLine(CONFM_MAX_LINE_SIZE);
	/* ahora llenamos la hash */
	while (data.size() != 0) {
		/* buscamos el '=' */
		eqPos = data.indexOf(CONFM_SEPARATOR);
		if (eqPos < 0) {
			debugp ("ConfigManipulator: Algun error en el archivo"
			" de configuracion\n");
			data = this->file.readLine(CONFM_MAX_LINE_SIZE);
			continue;
		}
		key = data.mid(0, eqPos);
		value = data.mid(eqPos + 1, data.size() - eqPos - 2 /*sin \n*/);
		/* agregamos esto al hash */
		this->hash.insert (key, value);
		
		/* leemos la proxima linea */
		data = this->file.readLine(CONFM_MAX_LINE_SIZE);
	}
}
	

/* Funcion que devuelve el valor de una variable (var).
* en caso de que var no exista => devuelve NULL.
* RETURNS:
*	value  	!= NULL (NOTE: alloca memoria)
*	NULL	si var no exista.
*/
QString * ConfigManipulator::getValue (QString & var)
{
	QString *result = NULL;
	
	if (var.isNull() == true) {
		ASSERT (false);
		return NULL;
	}
	result = new QString ("");
	
	/* buscamos */
	*result = this->hash.value (var, *result);
	
	if (*result == "") {
		/* no encontramos ni chota */
		delete result; result = NULL;
	}
	/* caso contrario => */
	return result;
}

/* Funcion que setea un valor (value) en una variable (var)
* dentro del archivo de configuracion.
* En caso de que var no exista => es creada una nueva entrada.
* REQUIRES:
*	var.isNull() == false
*	value.isNull() == false
*/
void ConfigManipulator::setValue (QString & var, QString value)
{
	if (var.isNull() || value.isNull()) {
		ASSERT (false);
		return;
	}
	/* agregamos a la hash (NOTE: reemplaza si ya existe la entrada) */
	this->hash.insert (var, value);
}

/* Funcion que guarda los cambios */
void ConfigManipulator::saveChanges(void)
{
	QHash<QString, QString>::const_iterator i;
	QString line = "";
	
	/* nos posicionamos al comienzo del archivo */
	this->file.close();
	this->file.open (QIODevice::WriteOnly);
	for (i = this->hash.constBegin(); i != this->hash.constEnd(); ++i) {
		/* ahora estasmoe iterando sobre cada uno de los elementos =>
		 * vamos a escribir todo el archivo d nuevo, total en teoria
		 * ya lo tenemos leido a todo */
		/*! si si, ineficiente y feo, pero no hay tiempo :( */
		line = i.key();
		line.append (CONFM_SEPARATOR);
		line.append (i.value());
		line.append ("\n");
		this->file.write (line.toUtf8());
	}
	
}


/*! DEBUG */
void ConfigManipulator::printConfVars(void)
{
	QHash<QString, QString>::const_iterator i;
	
	
	for (i = this->hash.constBegin(); i != this->hash.constEnd(); ++i) {
		printf ("KEY: %s\tVALUE: %s\n", qstrtochar(i.key()),
			 qstrtochar(i.value()));
	}
	
}
/* Destructor. Guarda todos los cambios en el archivo */
ConfigManipulator::~ConfigManipulator()
{
	saveChanges();
	this->hash.clear();
	this->file.close();
}