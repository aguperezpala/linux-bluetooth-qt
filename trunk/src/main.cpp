#include <QApplication>
#include <QMessageBox>

#include "controllerwindow.h"
#include "mainwidget.h"
#include "textwindow/textwindow.h"

int main(int argc, char *argv[])
{
    bool result = false;
    QApplication app(argc, argv);
    ControllerWindow test(0);
    //MainWidget xa(0);
    //xa.show();
    test.show();
    return app.exec();
}
