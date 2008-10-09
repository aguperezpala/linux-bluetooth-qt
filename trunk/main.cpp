#include <QApplication>
#include <QMessageBox>

#include "controllerwindow.h"
#include "loadwidget.h"

int main(int argc, char *argv[])
{
    bool result = false;
    QApplication app(argc, argv);
    ControllerWindow test(0);
    LoadWidget mainwidget(0);

    mainwidget.show();
    test.show();
    return app.exec();
}
