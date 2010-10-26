#include <QtGui/QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include "qtscrollwheel.h"
#include "widget.h"
#include "dcadataentrywidget.h"

#define USE_GV
#define test
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef test
    QGraphicsView view;
    QGraphicsScene scene;
    QGraphicsProxyWidget proxy;
    QtScrollWheel* scrollWheel = new QtScrollWheel;
    scrollWheel->setSkin("Beryl");
    scrollWheel->resize(QSize(80,200));
    scrollWheel->setMinimum(0);
    scrollWheel->setMaximum(999);
    scrollWheel->setSingleStep(1);
    proxy.setWidget(scrollWheel);
//    proxy.setFocus(Qt::MouseFocusReason);
//    proxy.setFlag(QGraphicsItem::ItemIsSelectable,true);
//    proxy.setFlag(QGraphicsItem::ItemIsMovable,true);
    scene.addItem(&proxy);
    view.setScene(&scene);
//    proxy.grabMouse();
    view.show();
    scrollWheel->grabMouse();
    scene.setFocusItem(&proxy);
#else
#ifdef USE_GV
    DCADataEntryWidget* dataEntryWidget = new DCADataEntryWidget;
    dataEntryWidget->showMaximized();
#else
    Widget w;
    w.show();
#endif
#endif
    return a.exec();
}
