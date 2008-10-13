#include "maintxtwindow.h"

MainTxtWindow::MainTxtWindow(QWidget *parent)
	: QWidget(parent)
{
	this->tw = NULL;
	this->msg = new QMessageBox();
	
	
	/*cargamos el archivo*/
	setupUi(this);
	
	/*cargamos la tabla de sms y la posicionamos dentro del layout*/
	smsTable = new SmsTable(0);
	verticalLayout_3->addWidget (smsTable);
	
	
	/*generamos la ventana donde se van a mostrar los sms*/
	this->tw = new TextWindow(0);
	if (this->tw == NULL) {
		msg->setText (QString ("Error al crear la ventana 'mostrar mensajes'");
		msg->show();
	}
		
	
	QMetaObject::connectSlotsByName(this); /*conectamos cada componente*/
}



void MainTxtWindow::on_txtbuttonSetSize_clicked()
{
	bool error = false;
	int xres = 0; int yres = 0;
	xres = txtsizeX->text().toInt (&error, 10);
	if (!error) {
		yres = txtsizeY->text().toInt (&error, 10);
		if (!error && )
			
		
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
