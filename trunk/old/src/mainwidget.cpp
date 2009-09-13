#include <QtUiTools>
#include <QtGui>
#include <QPixmap>


#include "mainwidget.h"


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    QUiLoader loader;

    QFile file("src/mainwidget.ui");
    file.open(QFile::ReadOnly);
    QWidget *formWidget = loader.load(&file, this);
    file.close();
    button1 = this->findChild<QPushButton *> ("button1");;
    QMetaObject::connectSlotsByName(this);


}

void MainWidget::on_button1_clicked ()
{
	button1->setText (QString ("JAJAJAJA"));
}
	