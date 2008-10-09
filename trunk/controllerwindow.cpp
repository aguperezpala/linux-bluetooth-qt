#include <QtGui>
#include <QTextEdit>
#include <QPushButton>
#include "controllerwindow.h"
#include <QMessageBox>
#include <stdio.h>
#include <iostream>

ControllerWindow::ControllerWindow(QWidget* parent): QWidget(parent)
{
	iw = new ImageWindow (0);
	text1 = new QTextEdit;
	text1->setReadOnly(false);
	text1->setLineWrapMode(QTextEdit::NoWrap);
	
	iw->setWindowFlags (SW_WITHOUT_BORDERS);
	iw->setWindowSize (300,300);
	iw->LoadImage( QString ("ona.jpg"));

	button1 = new QPushButton(tr("Mostrar imagen"));
	
	connect(button1, SIGNAL(clicked()), this, SLOT(SetImage()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(text1);
	layout->addWidget(button1);
	setLayout(layout);

	iw->show();
}

void ControllerWindow::SetImage ()
{	
	printf ("llego: %s \n",(char *) text1->toPlainText().toStdString().c_str());
	
	iw->LoadImage(text1->toPlainText());
}

