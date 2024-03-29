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


/* Constructor: Le vamos a pasar el archivo de configuracion
* que tiene que leer.
* REQUIRES;
*	fname.isNull == false
* NOTE: si el archivo no existe, es creado.
*/
MainWidget::MainWidget(QWidget *parent, QString & fname) : QWidget (parent)
{
	/* Pre */
	ASSERT (fname.isNull() == false);
	
	/* conectamos todas las funciones con los elementos del Widget */
	setupUi(this);

	
	this->confFile = fname;
	/* inicializamos todo lo demas */
	this->tw = NULL;
	this->twc = NULL;
	/*this->pw = NULL;
	this->pwc = NULL;*/
	this->table = NULL;
	this->realExit = false;
	this->dof = NULL;
	
	/* Creamos la tabla ahora */
	this->table = new DispObjTable (this);
	/* la configuramos (tamaño y la agregamos al Layout */
	this->layoutTable->addWidget (this->table);
	this->table->setMinimumSize (MW_TABLE_MIN_SIZE_X, MW_TABLE_MIN_SIZE_Y);
	
	/* creamos todo lo relacionado con el texto*/
	this->tw = new TextWindow (this->table);
	this->twc = new TxtWinControl (NULL, this->confFile, this->tw);
	this->tw->setFocusPolicy (Qt::StrongFocus);
	this->twc->setFocusPolicy (Qt::StrongFocus);
	
	this->tw->setWindowModality(Qt::NonModal);
	this->twc->setWindowModality(Qt::NonModal);
	
	
	/* creamos todo lo que tiene que ver con las pics */
	/*this->pw = new PictureWindow (this->table);
	this->pwc = new PicWinControl (NULL, this->confFile, this->pw);
	this->pw->setFocusPolicy (Qt::StrongFocus);
	this->pwc->setFocusPolicy (Qt::StrongFocus);
	this->pw->setWindowModality(Qt::NonModal);
	this->pwc->setWindowModality(Qt::NonModal);
	*/
	this->setFocusPolicy (Qt::StrongFocus);
	
	/* creamos el objetc filter */
	this->dof = new DispObjFilter();
	assert(this->dof != NULL);
	
	/* Recargamos todas las configuraciones */
	loadConfigs();
	
	/* reajustamos el widget */
	this->adjustSize();
	
	/* agregamos las ventanas de control al layout */
	this->textLayoutControl->addWidget (this->twc);
	/*this->picLayoutControl->addWidget (this->pwc);*/
	
	/* Mostramos todas las ventanas */
	this->tw->show();
	this->twc->show();
	/*this->pw->show();
	this->pwc->show();
	*/
	
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
	/* verificamos si esta pausado para autoactivarlo */
	if (this->tw->isPaused())
		this->tw->pause(false);
	
}




void MainWidget::on_btnDelSelected_clicked (void)
{
	int r = this->table->currentRow();
	
	if (0 <= r && r <= this->table->rowCount())
		this->table->deleteSelectedItem();
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
		destroyAll();
		event->accept();
	} else
		event->ignore();
	
}
void MainWidget::destroyAll (void)
{
	if (this->twc) {
		delete this->twc; this->twc = NULL;
	}
	if (this->tw) {
		delete this->tw; this->tw = NULL;
	}
	/*if (this->pwc) {
		delete this->pwc; this->pwc = NULL;
	}
	if (this->pw) {
		delete this->pw; this->pw = NULL;
	}*/
	if (this->table) {
		delete this->table; this->table = NULL;
	}
}




void MainWidget::acceptNewObject(DispObject *dobj)
{
	ASSERT(dobj != NULL);
	if(dobj == NULL)
		return;
	/* ahora nos fijamos si el operador lo acepta o no */
	if(this->dof->accept(dobj)) {
		/* si lo acepto => lo guardamos en la tabla */	
		addDispObject(dobj);
	} 
	
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
	destroyAll();
	
}


