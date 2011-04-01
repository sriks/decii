#include "mainwindow.h"

#include <QtGui/QApplication>
#include "testview.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    MainWindow mainWindow;
//    mainWindow.setOrientation(MainWindow::ScreenOrientationLockPortrait);
//    mainWindow.showExpanded();

    TestView v;
    v.showMaximized();

    return app.exec();
}
