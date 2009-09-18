#ifndef LOADWIDGET_H
#define LOADWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QtGui>
#include <QtUiTools>

QT_BEGIN_NAMESPACE
class QLabel;
class QSpinBox;
QT_END_NAMESPACE


class MainWidget : public QWidget
{
    Q_OBJECT

public:
	MainWidget(QWidget *parent = 0);
	
public slots:
	void on_button1_clicked ();

private:
	QPushButton *button1;
};

#endif
