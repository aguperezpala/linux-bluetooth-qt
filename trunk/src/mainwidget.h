#ifndef LOADWIDGET_H
#define LOADWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QSpinBox;
QT_END_NAMESPACE


class MainWidget : public QWidget
{
    Q_OBJECT

public:
	MainWidget(QWidget *parent = 0);

private:
};

#endif
