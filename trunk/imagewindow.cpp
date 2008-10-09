//
// C++ Implementation: imagewindow
//
// Description: 
//
//
// Author: Agustin Perez,,, <agustin@200-127-22-55>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "imagewindow.h"


ImageWindow::ImageWindow(QWidget *parent)
	: ShowWindow(parent) 
{
	//generamos las estructuras
	ql = new QLabel(parent, 0);
	image = new QPixmap();
	layout = new QVBoxLayout;
	
	
	
	/*vamos a determinar que no se "autoescale" las fotos*/
	ql->setScaledContents(true);
	/*le metemos la imagen al label*/
	ql->setPixmap(*image);
	//las cargamos en el layout
	layout->addWidget(ql);
	
	this->setLayout (layout); /*seteamos el layout del widget*/
	
}
bool ImageWindow::LoadImage (QString filename)
{
	bool result = false;
	
	result = image->load (filename, 0, Qt::AutoColor);
	
	ql->setPixmap(*image);
	
	return result;
}

ImageWindow::~ImageWindow()
{
}


