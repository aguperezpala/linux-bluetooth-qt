#ifndef MAINTXTWINDOW_H
#define MAINTXTWINDOW_H

#include <QWidget>
#include <QUiLoader>
#include <QMessageBox>
#include <QFontDialog>
#include <QFont>
#include <QMetaObject>
#include <assert.h>

#include "../filemanipulator/filemanipulator.h"
#include "ui_maintxtwindow.h"
#include "smstable.h"
#include "textwindow.h"
#include "../userspp/userobject.h"
#include "../userspp/userlist.h"


class MainTxtWindow : public QWidget, private Ui::MainTxtWindow 
{
	Q_OBJECT

public:
	MainTxtWindow(QWidget *parent = 0);
	~MainTxtWindow();
	
	
	/*Funcion que va a tomar un SmsObject y va a ser mostrado al usuario
	para saber si tiene que ser aceptado o no
		REQUIRES:
				sms != NULL
	
	Nota: NO DEBE SER LIBERADO el sms (es liberado luego automaticamente)
	*/
	void getExternSms (SmsObject* sms);
	
	/*Funcion que devuelve la usrList para poder usarla desde otros lugares
	como el modulo UserServer o algun otro lado
		RETURNS:
				NULL == NO EXISTE O ALGO RARO
				UserList == todo bien
	*/
inline	UserList* getUserList () {return this->usrlist;};
	

public slots:
	void on_txtbuttonSetSize_clicked();
	void on_txtbuttonSetPos_clicked();
	void on_txtbuttonHideWindow_clicked();
	void on_txtbuttonShowWindow_clicked();
	void on_txtbuttonSetVelocity_clicked();
	void on_txtbuttonDeleteSms_clicked();
	void on_txtbuttonStart_clicked();
	void on_txtbuttonStop_clicked();
	void on_txtbuttonSetFont_clicked();
	void on_txtbuttonSetBackColor_clicked();
	void on_txtbuttonSetFontColor_clicked();
	void on_txttextFileReciber_textChanged();
	void on_txtbuttonSetBetween_clicked();
	void on_smsTable_cellClicked (int r,int c){smsTable->setSelectedCurrent(r,c);};
	void new_sms_arrive(void);	/*que va a mostrar al usuario los sms almacenados
								  en la smslist*/


signals:
	void signal_new_sms (void);
			
			
private:
	/*esta funcion nos obtiene un sms desde un archivo
		REQUIRES:
			filename == parsed	(ya parseado)
	*/
	void getSmsFromFile (QString& filname);
	
	
	
	/*esta funcion nos va a mostrar un mensaje y nos dice si queremos o no
	 *encolarlo
		REQUIRES:
			data != NULL
		RETURNS:
			true == accept message
			false == error || !accept
	*/
	bool acceptSms (const QString& data);
	
	
	
	UserList *usrlist;
	SmsTable *smsTable; 	/*tabla para mostrar los sms*/
	TextWindow *tw;		/*ventana donde vamos a mostrar los sms*/
	QMessageBox *msg;	/*para mostrar algunos mensajes*/
	FileManipulator *fmanipulator;
	
	/*lista donde vamos almacenar temporalmente los mensajes que vamos a mostrar
	al administrador para que los "admita/desadmita"*/
	QList<SmsObject*> smslist;

};

#endif
