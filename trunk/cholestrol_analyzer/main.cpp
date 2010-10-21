#include <QtGui/QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "widget.h"
#include "dcadataentrywidget.h"

//#define USE_GV
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef USE_GV
    DCADataEntryWidget* dataEntryWidget = new DCADataEntryWidget;
    QGraphicsView view;
    QGraphicsScene scene;
    scene.addItem(dataEntryWidget);
    view.setScene(&scene);
    view.resize(360,640-200); //n8 size
    view.show();
#else
    Widget w;
    w.show();
#endif
    return a.exec();
}
