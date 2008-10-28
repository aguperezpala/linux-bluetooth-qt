#include "maintxtwindow.h"
#include <QtGui>
#include <QColor>
#include <QAbstractButton>
#include "smsobject.h"

MainTxtWindow::MainTxtWindow(QWidget *parent)
	: QWidget(parent)
{
	this->tw = NULL;
	this->msg = new QMessageBox(0);
	this->fmanipulator = NULL;
	this->usrlist = NULL;
	this->smsTable = NULL;
	
	/*cargamos el archivo*/
	setupUi(this);
	//QMetaObject::connectSlotsByName(this); /*conectamos cada componente*/
	
	/*cargamos la tabla de sms y la posicionamos dentro del layout*/
	smsTable = new SmsTable(0);
	assert (smsTable != NULL);
	verticalLayout_3->addWidget (smsTable);
	connect (smsTable,SIGNAL(cellClicked(int,int)),this,SLOT (on_smsTable_cellClicked(int,int)));
	
	/*generamos la ventana donde se van a mostrar los sms*/
	this->tw = new TextWindow(0, this->smsTable);
	if (this->tw == NULL) {
		msg->setText (QString ("Error al crear la ventana 'mostrar mensajes'"));
		msg->show();
		this->close();	/*!chequear esto*/
	}
	this->tw->setWindowFlags (SW_WITHOUT_BORDERS);
	this->tw->show();
	this->tw->setWindowSize (500,100); /*!fijarse que tiene que ser predeterminado*/
	
	/*generamos el filemanipulator*/
	this->fmanipulator = new FileManipulator;
	if (this->fmanipulator == NULL){
		msg->setText (QString ("Error al crear el FileManipulator"));
		msg->show();
		this->close();	/*!chequear esto*/
	}
		
	
	/*!creamos la userlist*/
	this->usrlist = new UserList();
	
	if (this->usrlist == NULL) {
		msg->setText (QString ("Error al crear la lista de usuarios"));
		msg->show();
		this->close();
	}
	
	/*!PRUIEBAAA*/
	this->tw->setMesg (QString ("Agustin daniel perez de los jodidididisimos andes"));
	//this->tw->setWindowFlags (SW_WITHOUT_BORDERS);
	
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
	this->smsTable->deleteSelectedItem();
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

void MainTxtWindow::on_txtbuttonSetBackColor_clicked()
{
	QColor color = QColorDialog::getColor(Qt::green, this);
	if (color.isValid()) {
		this->tw->setBackColor (color);
	}
}
void MainTxtWindow::on_txtbuttonSetFontColor_clicked()
{
	QColor color = QColorDialog::getColor(Qt::green, this);
	if (color.isValid()) {
		this->tw->setFontColor (color);
	}
}

void MainTxtWindow::on_txttextFileReciber_textChanged()
{
	QString aux = this->txttextFileReciber->toPlainText();
		
	if (!aux.isNull() && !aux.isEmpty()) {
		/* llamamos a la funcion que va a desencadenar todo el proceso*/
		getSmsFromFile (aux);
		this->txttextFileReciber->clear();
	}
	
}



bool MainTxtWindow::acceptSms (const QString& data)
{
	bool result = false;
	QMessageBox *msgFile = NULL;
	QAbstractButton *okbtn = NULL;
	QAbstractButton *cancelbtn = NULL;
	
	if (data.isNull() || data.isEmpty ()) {
		return false;
	} else {
		msgFile = new QMessageBox (this);
		
		if (msgFile != NULL) {
			okbtn = msgFile->addButton(tr("Aceptar"), QMessageBox::AcceptRole);
			cancelbtn = msgFile->addButton(tr("Cancelar"), QMessageBox::RejectRole);
			
			msgFile->setText (data);
			msgFile->exec();
			
			if (msgFile->clickedButton() == okbtn) {
				/*dprintf ("se acepto el elemento\n");*/
				result = true;
			} else { /*cualquier otro caso*/
					/*dprintf ("No se acepto el elemento\n");*/
					result = false;
			}
			delete okbtn;
			delete cancelbtn;
			delete msgFile;
		}
		
	}
	return result;
}

void MainTxtWindow::getSmsFromFile (QString& fn)
{
	QString filename(fn);	/*hacemos una copia por las dudas*/
	SmsObject *sms = NULL;	/*donde vamos a almacenar el Sms*/
	const QString *number = NULL;	/*aca vamos a almacenar momentaneamete el numero*/
	
	
	if (!filename.isNull() && !filename.isEmpty()) {
		
		this->fmanipulator->parseFilePath (filename);
		
		if (this->fmanipulator->getFileType (filename) != FileManipulator::FK_TEXT){
			dprintf ("Error de tipo de archivo, se espera un archivo de texto\n");
			msg->setText ("Error de tipo de archivo, se espera un archivo de texto");
			msg->show();
		} else {
			/*Todo esta perfecto entonces lo abrimos*/
			sms = this->fmanipulator->parseSmsFromFile (filename);
			if (sms != NULL) {
				/*ahora chequeamos que este en la lista*/
				number = sms->getNumber();
				if (number != NULL) {
					if (this->usrlist->existNumber(*number)) {
						/*!MOSTRAMOS EL MENSAJE PARA VER SI
						 *DEBE SER ENCOLADO O NO*/
						
						if (acceptSms (*(sms->getMesg()))) {
							/*!Debe ser encolado*/
							this->smsTable->insertBack (sms);
							/*ahora lo borramos*/
							/*this->fmanipulator->removeFile(filename);*//*!ANDA*/
							/*activamos el textwindow*/
							this->tw->signalNewMesg();
						} else {
							/*!se rechazo por el administrador
							 y lo borramos de la maquina*/
							/*lo borramos*/
							/*this->fmanipulator->removeFile(filename);*//*!ANDA*/
						}
					} else {
						/*!mensaje descartado porque no estaba registrado.. */
						dprintf ("Mensaje descartado porque no estaba registrado");
					}
				}
			}
		}
	this->txttextFileReciber->clear();
	}
}



MainTxtWindow::~MainTxtWindow()
{
	delete this->tw;
	delete this->smsTable;
	delete this->fmanipulator;
	delete this->msg;
}
