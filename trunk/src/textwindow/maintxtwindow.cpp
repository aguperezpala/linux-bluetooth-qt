#include "maintxtwindow.h"
#include <QtGui>

MainTxtWindow::MainTxtWindow(QWidget *parent)
	: QWidget(parent)
{
	this->tw = NULL;
	this->msg = new QMessageBox(0);
	
	
	
	/*cargamos el archivo*/
	setupUi(this);
	//QMetaObject::connectSlotsByName(this); /*conectamos cada componente*/
	
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
	this->tw->setWindowSize (500,100);
	
	
	/*!PRUIEBAAA*/
	this->tw->setMesg (QString ("Agustin daniel perez de los jodidididisimos andes"));
	
	
	
	
	
	
}



void MainTxtWindow::on_txtbuttonSetSize_clicked()
{
	bool ok = false;
	int xres = 0; int yres = 0;
	
	xres = txtsizeX->text().toInt (&ok, 10);
	if (ok) {
		yres = txtsizeY->text().toInt (&ok, 10);
		if (ok)
			this->tw->setWindowSize (xres, yres);
	}
}
	
void MainTxtWindow::on_txtbuttonSetPos_clicked()
{
	bool ok = false;
	int xres = 0; int yres = 0;
	
	xres = txtposX->text().toInt (&ok, 10);
	if (ok) {
		yres = txtposY->text().toInt (&ok, 10);
		if (ok)
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
	bool ok = false;
	int velocity = 0;
	
	velocity = txttextVelocity->text().toInt(&ok, 10);
	if (ok)
		this->tw->setVelocity (velocity);
	dprintf ("aaaasetvelocity\n");
}
void MainTxtWindow::on_txtbuttonDeleteSms_clicked()
{
}
void MainTxtWindow::on_txtbuttonStart_clicked()
{
	this->tw->start ();
}
void MainTxtWindow::on_txtbuttonStop_clicked()
{
	this->tw->stop();
}

void MainTxtWindow::on_txtbuttonSetFont_clicked()
{
	bool ok = false;
	dprintf ("a.\n");
	QFont font = QFontDialog::getFont(&ok,this->tw->getTextFont(), this->tw);
	if (ok) {
		this->tw->setTextFont (font);
	}
}