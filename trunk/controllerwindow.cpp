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
	tw = new TextWindow (0);
	
	/*configuramos ImageWindow*/
	//iw->setWindowFlags (SW_WITHOUT_BORDERS);
	iw->setWindowSize (300,300);
	iw->LoadImage( QString ("ona.jpg"));
	
	
	/*configuramos TextWindow*/
	//tw->setWindowFlags (SW_WITHOUT_BORDERS);
	tw->setWindowSize (800,300);
	
	text1 = new QTextEdit;
	text1->setReadOnly(false);
	text1->setLineWrapMode(QTextEdit::NoWrap);


	button1 = new QPushButton(tr("Mostrar imagen"));
	connect(button1, SIGNAL(clicked()), this, SLOT(SetImage()));
	buttontext = new QPushButton(tr ("set text"));
	connect(buttontext, SIGNAL(clicked()),this,SLOT(SetText()));
	QPushButton *buttontable = new QPushButton (tr("put table"));
	connect(buttontable, SIGNAL(clicked()),this,SLOT(SetItemTable()));
	
	qtw = new QTableWidget;
	
	qtw->setColumnCount (1);
	
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(text1);
	layout->addWidget(button1);
	layout->addWidget (buttontext);
	layout->addWidget(qtw);
	layout->addWidget (buttontable);
	setLayout(layout);

	iw->show();
	tw->show();
}

void ControllerWindow::SetImage ()
{	
	printf ("imagen: %s \n",(char *) text1->toPlainText().toStdString().c_str());
	iw->LoadImage(text1->toPlainText());
}
void ControllerWindow::SetText ()
{
	printf ("texto: %s \n",(char *) text1->toPlainText().toStdString().c_str());
	tw->setMesg( QString ("Agustin"), text1->toPlainText());
}

void ControllerWindow::SetItemTable ()
{
	QTableWidgetItem *aux = new QTableWidgetItem(text1->toPlainText(),0);
	qtw->insertRow(qtw->rowCount());
	printf ("currentRow: %d\n", qtw->currentRow());
	this->qtw->setItem (qtw->rowCount()-1,qtw->columnCount()-1, aux);
	qtw->resizeColumnToContents (qtw->columnCount()-1);
	qtw->resizeRowToContents (qtw->rowCount()-1);
	
}
