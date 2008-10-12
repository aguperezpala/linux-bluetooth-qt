#include <QApplication>
#include <QMessageBox>

#include "controllerwindow.h"
#include "mainwidget.h"
#include "textwindow/textwindow.h"
#include "textwindow/maintxtwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //ControllerWindow test(0);
    MainTxtWindow mtw(0);
    //MainWidget xa(0);
    
    mtw.show();
    //xa.show();
    //test.show();
    return app.exec();
}
