#include <QtGui/QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "widget.h"
#include "dcadataentrywidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DCADataEntryWidget* dataEntryWidget = new DCADataEntryWidget;
    QGraphicsView view;
    QGraphicsScene scene;
    scene.addItem(dataEntryWidget);
    view.setScene(&scene);
    view.resize(360,640-200); //n8 size
    view.show();

    return a.exec();
}
