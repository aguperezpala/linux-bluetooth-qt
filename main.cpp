#include <QApplication>

#include "showwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ShowWindow test(0);
    test.show();
    return app.exec();
}
