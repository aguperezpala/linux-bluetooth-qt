#include <QtGui>

#include "showwindow.h"


ShowWindow::ShowWindow(QWidget *parent)
    : QWidget(parent)
{
	/*default es normal*/
	QWidget::setWindowFlags(Qt::Window);
}
void ShowWindow::setWindowFlags(showWflags flags)
{
    if (flags == SW_WITH_BORDERS)
	    QWidget::setWindowFlags(Qt::Window);
    if (flags == SW_WITHOUT_BORDERS)
	    QWidget::setWindowFlags(Qt::FramelessWindowHint);
}
	
