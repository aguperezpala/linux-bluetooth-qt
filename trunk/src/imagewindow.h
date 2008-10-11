#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include "showwindow.h"
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QtGui>

class ImageWindow : public ShowWindow
{
public:
	/*derivamos el constructor de ShowWindow*/
     ImageWindow(QWidget *parent = 0);
     
     /*funcion para cargar imagenes, devuelve true si pudo cargar*/
     bool LoadImage (QString filename);
	
    ~ImageWindow();
    
private:
	QVBoxLayout *layout;	//el layout donde vamos a meter el Label (imagen)
	QPixmap *image;		//donde vamos a cargar la imagen
	QLabel *ql;		//donde vamos a mostrar la imagen

};

#endif
