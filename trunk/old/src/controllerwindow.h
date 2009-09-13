#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <QWidget>
#include <QTableWidget>

#include "imagewindow/imagewindow.h"
#include "textwindow/textwindow.h"
#include "qparser/parser.h"

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGroupBox;
class QLabel;
class QPushButton;
class QRadioButton;
QT_END_NAMESPACE


class ControllerWindow : public QWidget
{
    Q_OBJECT

public:
	ControllerWindow(QWidget* parent = 0);
	
	
public slots:
	void SetImage();
	void SetText();
	void SetItemTable();
	
private:
	
	ImageWindow *iw;
	TextWindow *tw;
	QTextEdit *text1;
	QPushButton *button1;
	QPushButton *buttontext;
	QTableWidget *qtw;	//test
	Parser *parser;

};


#endif
