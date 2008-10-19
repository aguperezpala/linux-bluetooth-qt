#ifndef MAINTXTWINDOW_H
#define MAINTXTWINDOW_H

#include <QWidget>
#include <QUiLoader>
#include <QMessageBox>
#include <QFontDialog>
#include <QFont>
#include <QMetaObject>

#include "../filemanipulator/filemanipulator.h"
#include "ui_maintxtwindow.h"
#include "smstable.h"
#include "textwindow.h"
/*#include "../users/user.h"
#include "../users/syncusrlist.h"
*/
class MainTxtWindow : public QWidget, private Ui::MainTxtWindow 
{
	Q_OBJECT

public:
	MainTxtWindow(QWidget *parent = 0);
	~MainTxtWindow();

private slots:
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
	void on_smsTable_cellClicked (int r,int c){smsTable->setSelectedCurrent(r,c);};
	

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
	bool acceptSms (QString& data);
	
	
	
	//syncusrlist_t *usrlist;	/*lista de usuarios habilitados*/
	SmsTable *smsTable; 	/*tabla para mostrar los sms*/
	TextWindow *tw;		/*ventana donde vamos a mostrar los sms*/
	QMessageBox *msg;	/*para mostrar algunos mensajes*/
	FileManipulator *fmanipulator;

};

#endif
