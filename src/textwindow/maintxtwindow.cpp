#include "maintxtwindow.h"
#include <QtGui>
#include <QColor>
#include <QAbstractButton>

MainTxtWindow::MainTxtWindow(QWidget *parent)
	: QWidget(parent)
{
	this->tw = NULL;
	this->msg = new QMessageBox(0);
	this->fmanipulator = NULL;
	
	
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
	QString *smsContent = NULL;
	QMessageBox *msgFile = NULL;
	QAbstractButton *okbtn = NULL;
	QAbstractButton *cancelbtn = NULL;
	//QTextEdit *txtfile = NULL;	/*dnd vamos a mostrar el archivo*/
	//QPushButton *btnOk = NULL;
	//QPushButton *btnCancel = NULL;
	//QVBoxLayout *flayout = NULL;
	
	if (!aux.isNull() && !aux.isEmpty()) {
		this->fmanipulator->parseFilePath (aux);
		
		if (!this->fmanipulator->getFileType (aux) == FileManipulator::FK_TEXT){
			dprintf ("Error de tipo de archivo, se espera un archivo de texto\n");
			msg->setText ("Error de tipo de archivo, se espera un archivo de texto");
			msg->show();
		} else {
			/*Todo esta perfecto entonces lo abrimos*/
			/*generamos todo los componentes*/
			msgFile = new QMessageBox (this);
			okbtn = msgFile->addButton(tr("Aceptar"), QMessageBox::AcceptRole);
			cancelbtn = msgFile->addButton(tr("Cancel"), QMessageBox::RejectRole);
			
			/*cargamos el contenido del archivo*/
			smsContent = this->fmanipulator->getFileContent (aux);
			
			
			
			if (smsContent != NULL)
				msgFile->setText (*smsContent);
			else
				dprintf ("El archivo estaba vacio\n");
			
			msgFile->exec();
			
			if (msgFile->clickedButton() == okbtn) {
				dprintf ("se acepto el elemento\n");
				/*encolamos*/
			
			} else { /*cualquier otro caso*/
				dprintf ("No se acepto el elemento\n");
			}
			delete smsContent;
			delete okbtn;
			delete cancelbtn;
			delete msgFile;
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
