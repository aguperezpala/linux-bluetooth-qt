#ifndef MAINTXTWINDOW_H
#define MAINTXTWINDOW_H

#include <QWidget>
#include <QUiLoader>
#include <QMessageBox>
#include "ui_maintxtwindow.h"
#include "smstable.h"
#include "textwindow.h"


class MainTxtWindow : public QWidget, private Ui::MainTxtWindow {

public:
	MainTxtWindow(QWidget *parent = 0);

public slots:
	void on_txtbuttonSetSize_clicked();
	void on_txtbuttonSetPos_clicked();
	void on_txtbuttonHideWindow_clicked();
	void on_txtbuttonShowWindow_clicked();
	void on_txtbuttonSetVelocity_clicked();
	void on_txtbuttonDeleteSms_clicked();
	void on_txtbuttonStart_clicked();
	void on_txtbuttonStop_clicked();
	

private:
	SmsTable *smsTable; 	/*tabla para mostrar los sms*/
	TextWindow *tw;		/*ventana donde vamos a mostrar los sms*/
	QMessageBox *msg;	/*para mostrar algunos mensajes*/

};

#endif
