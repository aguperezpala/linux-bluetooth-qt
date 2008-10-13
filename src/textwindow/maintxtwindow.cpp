#include "maintxtwindow.h"

MainTxtWindow::MainTxtWindow(QWidget *parent)
	: QWidget(parent)
{
	this->tw = NULL;
	this->msg = new QMessageBox(0);
	
	
	/*cargamos el archivo*/
	setupUi(this);
	
	/*cargamos la tabla de sms y la posicionamos dentro del layout*/
	smsTable = new SmsTable(0);
	verticalLayout_3->addWidget (smsTable);
	
	
	/*generamos la ventana donde se van a mostrar los sms*/
	this->tw = new TextWindow(0);
	if (this->tw == NULL) {
		msg->setText (QString ("Error al crear la ventana 'mostrar mensajes'"));
		msg->show();
		this->close();	/*!chequear esto*/
	}
	this->tw->show();
	
	
	/*!PRUIEBAAA*/
	this->tw->setMesg (QString ("Agustin daniel perez de los jodidididisimos andes"));
	this->tw->start();
	
	QMetaObject::connectSlotsByName(this); /*conectamos cada componente*/
}



void MainTxtWindow::on_txtbuttonSetSize_clicked()
{
	bool error = false;
	int xres = 0; int yres = 0;
	
	xres = txtsizeX->text().toInt (&error, 10);
	if (!error) {
		yres = txtsizeY->text().toInt (&error, 10);
		if (!error)
			this->tw->setWindowSize (xres, yres);
	}	
}
	
void MainTxtWindow::on_txtbuttonSetPos_clicked()
{
	bool error = false;
	int xres = 0; int yres = 0;
	
	xres = txtposX->text().toInt (&error, 10);
	if (!error) {
		yres = txtposY->text().toInt (&error, 10);
		if (!error)
			this->tw->setWindowPos (xres, yres);
	}
	
}
void MainTxtWindow::on_txtbuttonHideWindow_clicked()
{
	this->tw->setVisible (false);
}
void MainTxtWindow::on_txtbuttonShowWindow_clicked()
{
	this->tw->setVisible (true);
}
void MainTxtWindow::on_txtbuttonSetVelocity_clicked()
{
	bool error = false;
	int velocity = 0;
	
	velocity = txttextVelocity->text().toInt(&error, 10);
	if (!error)
		this->tw->setVelocity (velocity);
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
