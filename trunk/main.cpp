#include <QApplication>

#include "showwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ShowWindow test(0);
    test.setWindowFlags (SW_WITHOUT_BORDERS);
    test.setWindowSize (300,300);
    test.show();
    
    return app.exec();
}
