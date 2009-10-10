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
		this->mutex.lock();	/*! atomic */
		
		/* seteamos algunas propiedades */
		/*item->setFlags (Qt::ItemIsSelectable | Qt::ItemIsEnabled);*/
		/* creamos una columna al final */
		this->insertRow (this->rowCount());
		this->setItem (this->rowCount()-1,this->columnCount()-1,item);
		/* establecemos el tamaño contenido de los datos */
		result = true;
		
		this->mutex.unlock(); /*! atomic */
	}
	
	return result;
}
	

/* Constructor */
DispObjTable::DispObjTable(QWidget *parent)
	: QTableWidget (parent)
{
	/* Agregamos una simple columna */
	this->insertColumn(0);
	this->setColumnWidth(0, this->width());
	/* deshabilitamos la posibilidad de modificar los elementos */
	
	
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
	
	
	this->mutex.lock(); /*! atomic */
	/* si no esta dentro del rango volvemos */
	if (r <= this->rowCount() && r >= 0) {
		/* soltamos el candado */
		this->mutex.unlock();
		return;
	}

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
	
	this->mutex.unlock();
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
		{
			QByteArray data = "";
			QString realData = "";
			
			/* abrimos el archivo y lo lemos */
			if (obj->file.open (QIODevice::ReadOnly)) {
				data = obj->file.readAll();
				/* lo limpiamos de basuras como \n */
				data.replace ('\n', ' ');
				/* del tipo texto */
				realData = data;
				item = new QTableWidgetItem (realData);
				ASSERT (item != NULL);
				/* lo metemos en la tabla */
				enqueued = enqueueItem (item);
				obj->file.close();
			} else {
				/* cualquiera esto, lo borramos al choripan */
				delete obj; obj = NULL;
			}
			
			
		}
			break;
		case DISPOBJ_PICTURE:
		{
			QString picItem = "Foto: ";
			/* vamos a crear el item con el nombre de la foto */
			picItem.append (obj->file.fileName());
			item = new QTableWidgetItem (picItem);
			ASSERT (item != NULL);
			enqueued = enqueueItem (item);
		}
			break;
		default:
			/* some fucking big problem :( */
			ASSERT (false);
			return; /*! evitamos inconcistencias */
	}
	
	
	this->mutex.lock();
	/* lo agregamos a la cola */
	
	if (enqueued)
		this->queue.append (obj);
	
	this->mutex.unlock();
}


/* Funcion que simplemente muestra el primer elemento de la lista
* NOTE: NO lo saca de la misma, solo para poder "observarlo"
* RETURNS:
*	NULL		si no hay elementos.
*	const dobj != NULL 
*/
const DispObject * DispObjTable::getFirst (void)
{
	const DispObject * result = NULL;
	/*! hacemos esto atomico */
	this->mutex.lock();
	
	if (this->queue.isEmpty()) {
		this->mutex.unlock(); 
		return result;
	}
	/* si no es vacia devolvemos el primer elemento */
	result = this->queue.first();
	
	this->mutex.unlock(); 
	
	return result;
}


/* Funcion que saca un elemento de determinado tipo. Lo que hace es
* buscar en toda la lista, y el primero que encuentra de ese tipo
* de DispObject lo devuelve.
* REQUIRES:
*	type € dispObjKind_t
* RETURNS:
*	NULL	si no encuentra ninguno
*	dObj != NULL caso contrario
* ENSURES:
*	dobj->kind == type
* NOTE: Lo saca de la lista, quien lo llama es dueño del dObj.
*/
DispObject * DispObjTable::popFirst (dispObjKind_t type)
{
	QList<DispObject *>::iterator i;
	int pos = 0;
	DispObject * result = NULL;
	
	/*! hacemos atomico */
	this->mutex.lock();
	
	/* hacemos un simple chequeo antes para hacerlo mas eficiente */
	if (this->queue.isEmpty()) { 
		/* liberamos el candado */
		this->mutex.unlock();
		return result;
	}
	/* si no esta vacia => lo buscamos */
	
	for (i = this->queue.begin(); i != this->queue.end(); ++i) {
		/* ahora buscamos el primero de la lista que sea del tipo
		 * type */
		if ((*i) != NULL && (*i)->kind == type) {
				/* lo asignamos y salimos del siclo */
				result = *i;
				break;
		}
		pos++;
	}
	/* Verificamos que result != NULL para realmente eliminarlo de la 
	 * lista y tambien de la tabla */
	if (result != NULL) {
		/*! tenemos que borrarlo de la lista y la tabla */
		this->queue.removeOne(result);
		/* sacamos el elemento pos */
		this->removeRow (pos); /* automaticamente borra el item */
	}
	
	/* liberamos el candado */
	this->mutex.unlock();
	
	return result;
}


DispObject* DispObjTable::popFront ()
{
	DispObject *result = NULL;
	
	/*! hacemos atomico esto */
	this->mutex.lock();
	ASSERT (!this->queue.isEmpty());
	
	if (!this->queue.isEmpty()) {
		/* Obtenemos el primer elemento y lo liberamos */
		result = this->queue.first();
		this->queue.removeFirst();
		/* ahora removemos el primer elemento de la tabla */
		this->removeRow (0);
	}
	
	this->mutex.unlock();
	
	return result;
}


void DispObjTable::resizeEvent ( QResizeEvent * event )
{
	this->setColumnWidth(0, this->width());
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
	
