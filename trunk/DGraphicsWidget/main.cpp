#include <QtGui/QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QObject>
#include <QDebug>
#include <QGraphicsLinearLayout>
#include "dgraphicswidget.h"
#include "dgraphicstextwidget.h"
#include "dgraphicsview.h"

#define USE_DGRAPHICSVIEW
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DGraphicsWidget w;
    w.resize(400,400);
//    w.setTitleText("This day in history");
//    w.setTitlePixmap(QPixmap(":/resource/images/historychannel.jpeg"),true);

    QGraphicsLinearLayout layout(Qt::Vertical);
    DGraphicsTextWidget textitem("This is a nice text to show !!!!!! And let me check how it works with really big text!");
    layout.addItem(&textitem);
    textitem.textItem()->setTextWidth(w.rect().width() - 5);
    w.addContent(&layout);

#ifdef USE_DGRAPHICSVIEW

    DGraphicsView view;
    w.setParent(&view);
    view.scene()->addItem(&w);
    // Only widget in this window
    // so close window when widget is closed.
    qDebug()<<"connecting closebutton:"<<QObject::connect(&w,SIGNAL(closeButtonClicked()),
                                                    &a,SLOT(quit()));

    view.show();
#else
    QGraphicsScene scene;
    scene.addItem(&w);
    QGraphicsView view;
    view.setScene(&scene);
    scene.setSceneRect(w.rect());
    view.show();
#endif

    return a.exec();
}
