#include "mainwidget.h"



/* Funcion que guarda todas las variables de configuracion */
void MainWidget::saveConfigs(void)
{
	return;
}


/* Funcion que Carga todas las variables de configuracion */
void MainWidget::loadConfigs(void)
{
	return;
}


/*! Aca van las funciones que trabajan con la lista de DispObjects 
*  NOTE: Recordar que debemos borrar el archivo y el DispObject
*/
/* Funcion que entrega un mensaje para mostrar por pantalla. Esta
* es para la ventana de textos.
* RETURNS:
*	str	!= NULL	si es que existe algun mensaje
*	NULL	caso contrario.
* ENSURES:
*	en caso de enviar un mensaje este deja de existir en la tabla.
* NOTE: str ya no nos pertenece y debemos borrar ademas el archivo.
*/
QString * MainWidget::sendMsgSignal (void)
{
	DispObject * obj = NULL;
	QString * result = NULL;
	CUser * user = NULL;
	QByteArray data = "";
	
	/* sacamos de pecho el primer elemento que corresponda al tipo
	 * DISPOBJ_TEXT */
	obj = popFirst (DISPOBJ_TEXT);
	
	/* si es null => devolvemos NULL */
	if (obj == NULL)
		return NULL;
	
	/* Si no es null => sacamos la informacion y debemos eliminar el
	 * archivo y el DispObject */
	result = new QString("");
	/* un poco de seguridad... */
	if (result == NULL) {
		/*! estamos hasta el choripanaso mal */
		debugp ("MainWidget::sendMsgSignal: estamos hasta el choripan "
			" no hay memoria para crear un simple QString\n");
		/* borramos basura */
		delete obj;
		return result;
	}
		
	/* obtenemos el usuario para sacar el nickname */
	user = obj->getUser();
	if (user == NULL) {
		/*! implementamos alguna politica: nombre de usuario por
		 * default, por ejemplo.... sin nombre: :)
		 */
		(*result) = "sin nombre";
	} else
		/* si tenemos un usuario => obtenemos el nickname */
		(*result) = user->getNick();
	
	/* le agregamos unos 2 puntos y un espacio... */
	result->append (": ");
	/* ahora obtenemos los datos verdaderos, intentamos abrir el archivo
	 * y leer el contenido */
	if (obj->file.open (QIODevice::ReadOnly) == false) {
		/*! error abriendo el archivo, mierda carajo... muy raro */
		debugp ("MainWidget::sendMsgSignal: error abriendo archivo "
			"de texto, :(\n");
		/*! ACA debemos implementar otra politica: mostramos por
		 * pantalla algo? no mostramos nada? elegimos 2º */
		/* vamos a eliminar todo a la bosta, no nos sirve de nada */
		delete result; result = NULL;
		delete obj;
		return result;
	}
	/* si estamos aca es porque pudimos abrirlo => leemos los datos */
	data = obj->file.readAll();
	/* lo limpiamos de basuras como \n */
	data.replace ('\n', ' ');
	
	/* agregamos el mensaje finalmente al string */
	result->append (data);
	
	/* eliminamos la basura restante */
	delete obj;
	
	return result;
}
	


/* Funcion que entrega una imagen para mostrar por pantalla. Esta
* es para la ventana de pictures.
* RETURNS:
*	pic	!= NULL	si es que existe alguna imagen
*	NULL	caso contrario.
* ENSURES:
*	una vez mandado una picture, no se encuentra mas en la tabla
* NOTE: pic ya no nos pertenece y debemos borrar ademas el archivo.
*/
QPixmap * MainWidget::sendPicSignal (void)
{
	DispObject * obj = NULL;
	QPixmap * result = NULL;
	
	/* sacamos de pecho el primer elemento que corresponda al tipo
	* DISPOBJ_PICTURE */
	obj = popFirst (DISPOBJ_PICTURE);
	
	/* si es null => devolvemos NULL */
	if (obj == NULL)
		return NULL;
	
	/* ahora en teoria tenemos un objeto, lo vamos a cargar en la imagen */
	result = new QPixmap ();
	/* lo cargamos */
	if (result->load (obj->file.fileName()) == false) {
		/*! no pudimos cargarlo ni bosta */
		debugp ("MainWidget::sendPicSignal: no se pudo cargar la foto "
			"algun error interno :S... o que lo que\n");
		/* limpiamos la basura */
		if (result)
			delete result;
		delete obj; /* no nos sirve mas */
		return NULL; /* no vamos a poderla mostrar :( */
	}
	/* lo pudimos cargar correctamente :) => eliminamos el archivo ya que
	 * no nos hace mas falta, y ademas devolvemos la imagen */
	delete obj;
	
	return result;
}


/* Constructor: Le vamos a pasar el archivo de configuracion
* que tiene que leer.
* REQUIRES;
*	fname.isNull == false
* NOTE: si el archivo no existe, es creado.
*/
MainWidget::MainWidget(QWidget *parent = 0, QString & fname) : QWidget (parent)
{
	/* Pre */
	ASSERT (fname.isNull == false);
	
	this->confFile = fname;
	/* inicializamos todo lo demas */
	this->tw = NULL;
	this->twc = NULL;
	this->pw = NULL;
	this->pwc = NULL;
	this->table = NULL;
	this->realExit = false;
	/* creamos todo lo relacionado con el texto*/
	this->tw = new TextWindow (&sendMsgSignal);
	this->twc = new TxtWinControl (this, this->confFile, this->tw);
	
	/* creamos todo lo que tiene que ver con las pics */
	this->pw = new PictureWindow (&sendPicSignal);
	this->pwc = new PicWinControl (this, this->confFile, this->pw);
	
	/* Creamos la tabla ahora */
	this->table = new DispObjTable (this);
	/* la configuramos (tamaño y la agregamos al Layout */
	this->layoutTable->addWidget (this->table);
	this->table->setMinimumSize (MW_TABLE_MIN_SIZE_X, MW_TABLE_MIN_SIZE_Y);
	
	/* Recargamos todas las configuraciones */
	loadConfigs();
	
	/* reajustamos el widget */
	this->adjustSize();
	
	/* Mostramos todas las ventanas */
	this->tw->show();
	this->pw->show();
	this->tcw->show();
	this->pcw->show();
	this->show();
}

/* Funcion que agrega un elemento "mostrable" (DispObject).
* En teoria ya fue aceptado por el usuario.
* REQUIRES:
*	dobj	!= NULL
* NOTE: Una vez que se pasa el dobj NO debe ser eliminado el mismo.
*	 pasa a ser propiedad de esta clase.
*/
void MainWidget::addDispObject (DispObject * dobj)
{
	
	/* PRE */
	if (dobj == NULL) {
		ASSERT (false);
		/* no tenemos nada que hacer... */
		return;
	}
	
	this->table->insertBack(dobj);
}




void MainWidget::on_btnDelSelected_clicked (void)
{
	this->table->deleteSelectedItem();
}

void MainWidget::on_btnShowPic_clicked (void)
{
	this->pcw->show();
}

void MainWidget::on_btnShowTxt_clicked (void)
{
	this->tcw->show();
}


void MainWidget::on_btnExit_clicked()
{
	/* cerramos realmente... */
	this->realExit = true;
	this->close();
}

void MainWidget::closeEvent(QCloseEvent *event)
{
	if (this->realExit) {
		/* guardamos la configuracion */
		saveConfigs();
		event->accept();
	} else
		event->ignore();
	
}




/* Destructor, esto destruye ### TODAS ### las ventanas, tener
* cuidado con esto.
*/
MainWidget::~MainWidget()
{
	
	/* guardamos las configuraciones */
	saveConfigs();
	/* limpiamos toda la memoria correspondiente =>
	 * Las ControlWindow y sus respectivos ventanas
	 */
	if (this->twc) {
		delete this->twc; this->twc = NULL;
	}
	if (this->tw) {
		delete this->tw; this->tw = NULL;
	}
	if (this->pwc) {
		delete this->pwc; this->pwc = NULL;
	}
	if (this->pw) {
		delete this->pw; this->pw = NULL;
	}
	if (this->table) {
		delete this->table; this->table = NULL;
	}
}


