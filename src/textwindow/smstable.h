#ifndef SMSTABLE_H
#define SMSTABLE_H

#include <QTableWidget>
#include <QQueue>

#include "smsobject.h"
#include "../debug.h"


QT_BEGIN_NAMESPACE
class QTableView;
QT_END_NAMESPACE


class SmsTable : public QTableWidget
{
    Q_OBJECT

public:
	SmsTable(QWidget *parent = 0);

	/*removemos el item en la fila r */
	void deleteItem (int r);
	void deleteSelectedItem ();
	
	SmsObject* popFront ();	/*tambien elimina el item*/
	void insertBack (SmsObject*);
inline	void setSelectedCurrent (int r, int c){this->setCurrentCell (r,c);};

		
public slots:
	void on_smsTable_cellClicked (int,int);
	
signals: /*estas son las señales que vamos a usar para esta tabla*/
	void smsTable_clicked (int,int);

private:
	QQueue<SmsObject *> queue;	/*cola donde vamos a almacenar los sms*/
	
	
};


#endif
