#include <QtGui/QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>

#include <QGraphicsLinearLayout>
#include "dgraphicswidget.h"
#include "dgraphicstextitem.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsScene scene;
    DGraphicsWidget w;
    w.resize(400,100);
    w.setTitleText("DWidget Title");

    QGraphicsLinearLayout layout(Qt::Vertical);
    DGraphicsTextItem textitem("This is a nice text to show !!!!!! And let me check how it works with really big text!");
    layout.addItem(&textitem);
    textitem.textItem()->setTextWidth(w.rect().width() - 5);
    w.addContentLayout(&layout);

    scene.addItem(&w);
    QGraphicsView view;
    //view.setMaximumSize(w.size().toSize());
    view.setScene(&scene);
    scene.setSceneRect(w.rect());
    view.show();
    return a.exec();
}
