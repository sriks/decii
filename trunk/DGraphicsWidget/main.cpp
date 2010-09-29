#include <QtGui/QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>

#include <QGraphicsLinearLayout>
#include "dgraphicswidget.h"
#include "dgraphicstextwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsScene scene;
    DGraphicsWidget w;
    w.resize(400,400);
    w.setTitleText("This day in history");
    w.setTitlePixmap(QPixmap(":/resource/images/historychannel.jpeg"),true);

    QGraphicsLinearLayout layout(Qt::Vertical);
    DGraphicsTextWidget textitem("This is a nice text to show !!!!!! And let me check how it works with really big text!");
    layout.addItem(&textitem);
    textitem.textItem()->setTextWidth(w.rect().width() - 5);
    w.addContent(&layout);

    scene.addItem(&w);
    QGraphicsView view;
    //view.setMaximumSize(w.size().toSize());
    view.setScene(&scene);
    scene.setSceneRect(w.rect());
    view.show();
    return a.exec();
}
