#include <QtGui>

#include "showwindow.h"


ShowWindow::ShowWindow(QWidget *parent)
    : QWidget(parent)
{/*
    textEdit = new QTextEdit;
    textEdit->setReadOnly(true);
    textEdit->setLineWrapMode(QTextEdit::NoWrap);

    closeButton = new QPushButton(tr("&Close"));
    QPushButton *agu1 = new QPushButton(tr("AAAA"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(textEdit);
    layout->addWidget(closeButton);
    layout->addWidget(agu1);
    
    setLayout(layout);

    setWindowTitle(tr("Preview"));*/
}
void ShowWindow::setWindowFlags(showWflags flags)
{
    if (flags == SW_WITH_BORDERS)
	    QWidget::setWindowFlags(Qt::Window);
    if (flags == SW_WITHOUT_BORDERS)
	    QWidget::setWindowFlags(Qt::FramelessWindowHint);
}
	
