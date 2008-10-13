#ifndef SMSTABLE_H
#define SMSTABLE_H

#include <QTableWidget>
//#include "smsobject.h"


QT_BEGIN_NAMESPACE
class QTableView;
QT_END_NAMESPACE


class SmsTable : public QTableWidget
{
    Q_OBJECT

public:
    SmsTable(QWidget *parent = 0);
    
    void deleteItem (int r);/*removemos el item en la fila r */
    void deleteSelectedItem ();
    
    //void insertFront (SmsObject&);
    
    

		
public slots:
	void on_smsTable_cellClicked (int,int);
	
signals: /*estas son las señales que vamos a usar para esta tabla*/
	void smsTable_clicked (int,int);
    
private:
	
};


#endif
