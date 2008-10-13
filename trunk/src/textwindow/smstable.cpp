#include <QtGui>

#include "smstable.h"


SmsTable::SmsTable(QWidget *parent)
	: QTableWidget (parent)
{
	this->insertRow(0);
	this->insertColumn(0);
	connect (this, SIGNAL (cellClicked (int,int)),this, SIGNAL (smsTable_clicked(int,int)));
	
}


void SmsTable::deleteItem (int r, int c)
{
	this->removeRow (r,c);
}
	
void SmsTable::deleteSelectedItem ()
{
	this->removeRow (this->currentRow(),this->currentColumn());
}






/*********************	SLOTS	**********************************/

void SmsTable::on_smsTable_cellClicked (int r, int c)
{
	this->setCurrentCell (r,c);
}
	