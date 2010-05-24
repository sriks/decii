#include <QtGui/QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QKeyEvent>
#include <QGraphicsBlurEffect>
#include "clockwidget.h"
#include "analoghand.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClockWidget clockWidget;
    clockWidget.showMaximized();
    return a.exec();
}


//system("rundll32.exe user32.dll, LockWorkStation");
