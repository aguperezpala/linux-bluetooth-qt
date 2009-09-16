/*!objetos que van a ser mostrados en el widget.
 * Mas o menos se caracteriza como
 *	struct marquesinobj {
 *		int showPos;	--posicion desde la que tiene que ser 
 *				  dibujada en el widget
 *		int lengh;	--cantidad de caracteres
 *		QString* data;	--texto a ser dibujado
 *	}
*/
#ifndef MARQUESINOBJ_H
#define MARQUESINOBJ_H

#include <QString>
#include "../../debug.h"
class MarquesinObj {

public:
	bool addSomeOne;	/* determina si ya agrego a alguien o no */
	
	/* Constructor:
	 * REQUIRES:
	 *	str != NULL
	 *	showPos >= 0	(posicion en la que se va a encontrar en la
	 *			 pantalla)
	 * NOTE: No se debe liberar el str, Pertenece a la estructura ahora :)
	 */
	MarquesinObj(QString* str, int showPos);
	
	inline	int getShowPos(){return this->showPos;};
	inline	void setShowPos(int p){this->showPos = p;};
	
	
	/* No debe ser liberado este data */
	inline const QString* getData () {return this->data;};
	
	
	/* Funcion que setea los datos, en caso de que ya existiese un dato
	 * lo borra y se le asigna el nuevo.
	 * REQUIRES:
	 *	d != NULL
	 */
	void setData (QString *d);
	
	/* Funcion que devuelve el tamaño.
	 * RETURNS:
	 *	-1 		if no data exists
	 *	length		otherwise
	 */
	int getDataSize ();
	
	
	/* Destructor:
	 * Libera toda la memoria
	 */
	~MarquesinObj();
	
private:
	
	QString *data;
	int showPos;
	
};





#endif
