#include <QtGui/QApplication>
#include "horoscopewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    horoscopewidget w;
    w.show();
    return a.exec();
}
