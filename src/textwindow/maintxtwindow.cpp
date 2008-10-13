#include "maintxtwindow.h"

MainTxtWindow::MainTxtWindow(QWidget *parent)
	: QWidget(parent)
{
	/*cargamos el archivo*/
	setupUi(this);
	
	/*cargamos la tabla de sms y la posicionamos dentro del layout*/
	smsTable = new SmsTable(0);
	verticalLayout_3->addWidget (smsTable);
	
	QMetaObject::connectSlotsByName(this); /*conectamos cada componente*/
}



void MainTxtWindow::on_txtbuttonSetSize_clicked()
{
}
	
void MainTxtWindow::on_txtbuttonSetPos_clicked()
{
}
void MainTxtWindow::on_txtbuttonHideWindow_clicked()
{
}
void MainTxtWindow::on_txtbuttonShowWindow_clicked()
{
}
void MainTxtWindow::on_txtbuttonSetVelocity_clicked()
{
}
void MainTxtWindow::on_txtbuttonDeleteSms_clicked()
{
}
void MainTxtWindow::on_txtbuttonStart_clicked()
{
}
void MainTxtWindow::on_txtbuttonStop_clicked()
{
}
