#ifndef SMSTABLE_H
#define SMSTABLE_H

#include <QTableWidget>



QT_BEGIN_NAMESPACE
class QTableView;
QT_END_NAMESPACE


class SmsTable : public QTableWidget
{
    Q_OBJECT

public:
    SmsTable(QWidget *parent = 0);
    /*para setear el tipo de ventana*/	
    
private:
};


#endif
