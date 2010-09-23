#include <QtGui/QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "dgraphicswidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsScene scene;
    DGraphicsWidget w;
    w.resize(200,100);
    w.setTitleText("DWidget Title");
    scene.addItem(&w);
    QGraphicsView view;
    view.setScene(&scene);
    view.show();
    return a.exec();
}
