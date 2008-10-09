#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <QWidget>

#include "imagewindow.h"

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
private:
	ImageWindow *iw;
	QTextEdit *text1;
	QPushButton *button1;

};


#endif
