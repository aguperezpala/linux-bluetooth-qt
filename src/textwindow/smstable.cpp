#include <QtGui>
#include <QTableWidgetItem>

#include "smstable.h"


SmsTable::SmsTable(QWidget *parent)
	: QTableWidget (parent)
{
	this->insertColumn(0);
	connect (this, SIGNAL (cellClicked (int,int)),this, SIGNAL (smsTable_clicked(int,int)));
	
}


void SmsTable::deleteItem (int r)
{
	SmsObject *aux = NULL;
	if (r <= this->rowCount() && r >= 0){ /*tiene que estar dentro del rango*/
		this->removeRow (r); /*automaticamente borra el item*/
		
		aux = this->queue.dequeue();
		if (aux != NULL) {
			delete aux;
			aux = NULL;
		}
	}
	
}
	
void SmsTable::deleteSelectedItem ()
{
	deleteItem (this->currentRow());
}


void SmsTable::insertBack (SmsObject* sms)
{
	QTableWidgetItem *item = NULL;
	
	if (sms != NULL) {
		/*encolamos*/
		this->queue.enqueue (sms);
		
		/*vamos a mostrar el mensaje en la tabla*/
		this->insertRow (this->rowCount());
		
		dprintf ("Estamos encolando : %s\n",(char *) (*(sms->getName())).toStdString().c_str());
		item = new  QTableWidgetItem (*(sms->getMesg()),0);
		if (item != NULL){
			this->setItem (this->rowCount()-1,this->columnCount()-1,item);
			/*establecemos el tamaño del sms*/
			this->resizeColumnToContents (this->columnCount()-1);
			this->resizeRowToContents (this->rowCount()-1);
		}
		else
			dprintf ("No se pudo crear el item en insertItem\n");
		
	} else {
		dprintf ("Recibimos un sms NULL en insertBack\n");
	}
}

SmsObject* SmsTable::popFront ()
{
}



/*********************	SLOTS	**********************************/

void SmsTable::on_smsTable_cellClicked (int r, int c)
{
	this->setCurrentCell (r,c);
}
