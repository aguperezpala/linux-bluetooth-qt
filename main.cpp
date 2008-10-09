#include <QApplication>
#include <QMessageBox>
#include "controllerwindow.h"
#include <unistd.h>
int main(int argc, char *argv[])
{
    bool result = false;
    QApplication app(argc, argv);
    ControllerWindow test(0);
    QMessageBox msg(0);
    
    
    test.show();
    if (result)
	    msg.setText (QString ("FUNCIONO"));
    else
	    msg.setText (QString (" NOOOOOOOOO FUNCIONO"));
    
    
    msg.show();
    
    
    
    //test.LoadImage( QString ("nonilis.JPG"));
    
    return app.exec();
}
