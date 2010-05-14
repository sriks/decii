#include <QtGui/QApplication>
#include "horoscope.h"
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsView gv;
    QGraphicsScene scene;
    Horoscope w;
    scene.addItem(&w);
    gv.setScene(&scene);
    gv.show();
    // tell that we want a see through background
    /*w.setAttribute(Qt::WA_TranslucentBackground, true);
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.resize(QSize(200,320));
    w.show();*/


    return a.exec();
}
