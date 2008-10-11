#include <QtGui>
#include <QTextEdit>
#include <QPushButton>
#include <QFileInfo>
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
	iw->LoadImage( QString ("../ona.jpg"));
	
	
	/*configuramos TextWindow*/
	//tw->setWindowFlags (SW_WITHOUT_BORDERS);
	tw->setWindowSize (800,300);
	
	text1 = new QTextEdit;
	text1->setReadOnly(false);
	text1->setAcceptRichText (false);
	//text1->setLineWrapMode(QTextEdit::NoWrap);


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
	QFileInfo fi(text1->toPlainText());
	QString aux;
	QString aux1;
	aux = text1->toPlainText();
	parseFile (aux);
	aux1 = fi.canonicalFilePath();
	//aux1.append (fi.fileName());
	printf ("fi: %s \n",(char *) aux1.toStdString().c_str());
	printf ("imagen: %s \n",(char *) aux.toStdString().c_str());
	text1->setText(text1->toHtml());
	iw->LoadImage(aux);
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

void ControllerWindow::parseFile (QString& str)
{
	bool cond = false;
	int i = 0;
	
	if (!str.isNull() && !str.isEmpty()) {//chequeamos por las dudas
		while (!cond && i < (str.length()-1)) {
			cond = ((str[i] == QChar ('/')) && (str[(i+1)] != QChar ('/')));
			i++;
		}
		//en este punto deberiamos tener i = primera / del verdadero path
		str.remove (0, i-1); //sacamos la "basura"
		str.replace ("%20"," "); //reemplazamos los espacios
		str.replace ("\n", ""); //sacamos los "enters"
		str.trimmed (); //sacamos los espacios al final y al principio
	}
}


