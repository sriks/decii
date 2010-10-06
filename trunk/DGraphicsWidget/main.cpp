#include <QDebug>
#include <QtGui/QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QApplication>
#include "dgraphicswidget.h"
#include "dgraphicstextwidget.h"
#include "dgraphicsview.h"

#define USE_DGRAPHICSVIEW
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DGraphicsWidget* w = new DGraphicsWidget;
    w->setTitleText("This day in history");
    QFont font("arial",20);
    font.setBold(true);
    w->setTitleFont(font);
    w->setTitlePixmap(QPixmap(":/resource/images/qt.png"),true);

    DGraphicsTextWidget* content = new DGraphicsTextWidget("Testing with really long text. Lets see what happens if this text is reallllllllllllllllllllllllllllllllllly long!",
                                                           w);
    w->addContent(content);

#ifdef USE_DGRAPHICSVIEW
    DGraphicsView view;
    w->setParent(&view);
    view.scene()->addItem(w);
    qDebug()<<"connected closeButtonClicked:"<<
            QObject::connect(w,SIGNAL(closeButtonClicked()),&view,SLOT(close()));

//#else
//    QGraphicsScene scene;
//    scene.addItem(&w);
//    QGraphicsView view;
//    //view.setMaximumSize(w.size().toSize());
//    view.setScene(&scene);
//    scene.setSceneRect(w.rect());
//    qDebug()<<"connected closeButtonClicked:"<<
//            QObject::connect(&w,SIGNAL(closeButtonClicked()),&a,SLOT(closeAllWindows()));

#endif
#ifndef Q_OS_SYMBIAN
    view.show();
#else
    view.showMaximized();
#endif
    return a.exec();
}
