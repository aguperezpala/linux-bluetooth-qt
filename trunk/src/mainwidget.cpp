#include <QtUiTools>
#include <QtGui>
#include <QPixmap>

#include "loadwidget.h"


LoadWidget::LoadWidget(QWidget *parent)
    : QWidget(parent)
{
    QPixmap aguPix;
    QUiLoader loader;

    QFile file("src/mainwidget.ui");
    file.open(QFile::ReadOnly);
    QWidget *formWidget = loader.load(&file, this);
    file.close();
    QMetaObject::connectSlotsByName(this);


}

