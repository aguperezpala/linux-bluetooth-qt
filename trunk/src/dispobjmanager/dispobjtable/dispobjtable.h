/*! Esta tabla lo que va hacer es mostrar todos los DispObjects que se estan
 * por mostrar en las relativas "ventanas" (txtwindow o picturewindow -futuro-).
 * NOTE: es una tabla con soporte a multiples concurrencias => acciones son
 * 	 atomicas.
*/
#ifndef DispObjTable_H
#define DispObjTable_H

#include <QTableWidget>
#include <QList>
#include <QMutex>

#include "../../dispobject/dispobject.h"
#include "../../debug.h"


QT_BEGIN_NAMESPACE
class QTableView;
QT_END_NAMESPACE


class DispObjTable : public QTableWidget
{
    Q_OBJECT

public:
	/* Constructor */
	DispObjTable(QWidget *parent = 0);
	
	/* Destructor:
	* NOTE: Remueve (libera memoria) todos los DispObjects que tenga en la
	* 	tabla 
	*/
	~DispObjTable();

	/* Funcion que remueve el DispObject que se encuentra en la fila r
	 * liberando su memoria y sacandolo de la cola.
	 * REQUIRES:
	 *	0 <= r <= this->rowCount()
	 */
	void deleteItem (int r);
	
	/* idem: lo que hace es deleteItem(selectedItem); */
	void deleteSelectedItem ();
	
	
	/* Funcion que simplemente muestra el primer elemento de la lista
	 * NOTE: NO lo saca de la misma, solo para poder "observarlo"
	 * RETURNS:
	 *	NULL		si no hay elementos.
	 *	const dobj != NULL 
	 * NOTE: no manipilarlo ni borrarlo!!!
	 */
	const DispObject * getFirst (void);
	
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
	DispObject * popFirst (dispObjKind_t type);
	
	/* Funcion que extrae el primer elemento de la tabla (lo saca de la
	 * misma) y devuelve el puntero al mismo,
	 * REQUIRES:
	 *	!this->isEmpty()
	 * RETURNS:
	 *	dispObj 	!= NULL  if success
	 *	dispObj		== NULL	 on error
	 * NOTE: El que pidio el objeto es el responsable de liberarlo ;)
	 */
	DispObject* popFront ();
	
	/* Funcion que inserta al final de la tabla un DispObject
	 * REQUIRES:
	 * 	obj != NULL
	 * NOTE: obj NO debe ser eliminado una vez insertado, pertenece
	 *	a la tabla.
	 *//*! Aca deberiamos chequear si es de tipo imagen o texto para
	  * agregarlos de diferentes maneras a la lista, por el momento
	  * solo vamos aceptar del tipo texto :) 
	  */
	void insertBack (DispObject* obj);
	
	
public slots:
	virtual void on_smsTable_cellClicked (int,int);
	
	
	
signals: 
	void cellClicked (int,int);

private:
				/* Funciones */
	/* Esta funcion simplemente selecciona el elemento actual de la
	 * tabla 
	 */
	inline	void setSelectedCurrent (int r, int c)
	{this->setCurrentCell(r,c);};
		
	/* Funcion que agrega un QTableWidgetItem al final de la tabla.
	 * Solo facilita las cosas :)
	 * REQUIRES:
	 *	item != NULL
	 * RETURNS:
	 *	true	if success
	 *	false 	otherwise
	 */
	bool enqueueItem (QTableWidgetItem * item);
	
				/* Atributos */
	/* cola donde vamos a almacenar los sms */
	QList<DispObject *> queue;
	QMutex mutex;	/* para hacer atomicas las operaciones */
	
};


#endif
