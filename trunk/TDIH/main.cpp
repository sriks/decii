#include <QtGui/QApplication>
#include "tdih.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TDIH w;
    w.showFullScreen();
    return a.exec();
}
