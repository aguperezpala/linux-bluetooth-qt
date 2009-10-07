#include <QtGui>
#include <QTableWidgetItem>
#include "dispobjtable.h"



/*********************	SLOTS	**********************************/

void DispObjTable::on_smsTable_cellClicked (int r, int c)
{
	this->setCurrentCell (r,c);
}


/* Funcion que agrega un QTableWidgetItem al final de la tabla.
* Solo facilita las cosas :)
* REQUIRES:
*	item != NULL
* RETURNS:
*	true	if success
*	false 	otherwise
*/
bool DispObjTable::enqueueItem (QTableWidgetItem * item)
{
	bool result = false;
	
	/*	PRE	*/
	ASSERT (item != NULL);
	
	
	if (item != NULL) {
		/* creamos una columna al final */
		this->insertRow (this->rowCount());
		this->setItem (this->rowCount()-1,this->columnCount()-1,item);
		/* establecemos el tamaño contenido de los datos */
		this->resizeColumnToContents (this->columnCount()-1);
		this->resizeRowToContents (this->rowCount()-1);
		result = true;
	}
	
	return result;
}
	

/* Constructor */
DispObjTable::DispObjTable(QWidget *parent)
	: QTableWidget (parent)
{
	/* Agregamos una simple columna */
	this->insertColumn(0);
	/* Conectamos la señal que cuando clickeen una celda nos llamen a
	 * nuestra funcion smsTable_clicked */
	connect (this, SIGNAL (cellClicked (int,int)),this, 
		  SLOT(on_smsTable_cellClicked(int,int)));
}


void DispObjTable::deleteItem (int r)
{
	DispObject *aux = NULL;
	
	/* Pre */
	ASSERT (0 <= r && r <= this->rowCount());
	
	/* si no esta dentro del rango volvemos */
	
	if (r <= this->rowCount() && r >= 0)
		return;

	this->removeRow (r); /* automaticamente borra el item */
	
	/*! Veamos que debe existir una correspondencia univoca entre cada fila
	 *  i y el elemento i de la lista, osea, hay una funcion biyectiva
	 * entre la lista y la tabla.
	 */
	ASSERT (r <= this->queue.size());	/*! NOTE: DEBE CUMPLIRSE */
	aux = this->queue[r];
	if (aux != NULL) {
		delete aux;	/* se borra el archivo */
		aux = NULL;
		/* lo sacamos de la "cola" */
		this->queue.removeAt(r);
	}
}
	
void DispObjTable::deleteSelectedItem ()
{
	deleteItem (this->currentRow());
}

/* Funcion que inserta al final de la tabla un DispObject
* REQUIRES:
* 	obj != NULL
*/
void DispObjTable::insertBack (DispObject* obj)
{
	QTableWidgetItem *item = NULL;
	bool enqueued = false;
	
	/* Pre */
	ASSERT (obj != NULL);
	
	/* si no existe volvemos */
	if (obj == NULL) return;
	
	/*! ahora chequeamos si es del tipo imagen o del tipo texto */
	
	switch (obj->kind) {
		case DISPOBJ_TEXT:
			/* del tipo texto */
			item = new QTableWidgetItem (obj->getData());
			ASSERT (item != NULL);
			/* lo metemos en la tabla */
			enqueued = enqueueItem (item);
			break;
		case DISPOBJ_PICTURE:
			/* deberiamos generar un item de forma especial */
			
			break;
			
		default:
			/* some fucking big problem :( */
			ASSERT (false);
	}
	
	/* lo agregamos a la cola */
	this->queue.append (obj);
}

DispObject* DispObjTable::popFront ()
{
	DispObject *result = NULL;
	
	ASSERT (!this->queue.isEmpty());
	
	if (!this->queue.isEmpty()) {
		/* Obtenemos el primer elemento y lo liberamos */
		result = this->queue.first();
		this->queue.removeFirst();
		/* ahora removemos el primer elemento de la tabla */
		this->removeRow (0);
	}
	
	return result;
}


/* Destructor */
DispObjTable::~DispObjTable()
{
	DispObject *obj = NULL;
	
	while (!this->queue.isEmpty()) {
		/* obtenemos y liberamos los DispObjects */
		obj = this->queue.first();
		if (obj != NULL) {
			delete obj;
		}
		obj = NULL;
		this->queue.removeFirst();
		/* liberamos la tabla */
		this->removeRow(0);
	}
}
	
