/*!objetos que van a ser mostrados en el widget.Mas o menos se caracteriza como
struct marquesinobj {
		int showPos;	--posicion desde la que tiene que ser dibujada en el widget
		int lengh;		--cantidad de caracteres
		QString* data;	--texto a ser dibujado
}
*/
#ifndef MARQUESINOBJ_H
#define MARQUESINOBJ_H

#include <QString>

class MarquesinObj {

public:
	bool addSomeOne;		/*determina si ya agrego a alguien o no*/
	
	/*constructor
			REQUIRES:
					str != NULL
					showPos >= 0
	*/
	MarquesinObj(QString* str, int showPos);
	
	inline	int getShowPos(){return this->showPos;};
	inline	void setShowPos(int p){this->showPos = p;};
	inline	QString* getData () {return this->data;};
	inline	void setData (QString *d) {this->data = d;};
	
	/*devuelve el tamaño del string*/
	int getDataSize ();
	
	
	
	~MarquesinObj();
	
private:
	
	QString *data;
	int showPos;
	
};





#endif
