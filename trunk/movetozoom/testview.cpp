#include "testview.h"
#include "MoveToZoom.h"
#include <QDebug>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
TestView::TestView(QWidget* parent)
         :QGraphicsView(parent)
{
    QGraphicsScene* s = new QGraphicsScene(this);
    this->setScene(s);
    item = s->addText("Hello world!");
    item->setPos(50,50);
    item->setDefaultTextColor(QColor(Qt::white));
    MoveToZoom* zm = new MoveToZoom(item,this);
    connect(zm,SIGNAL(zoom(qreal)),this,SLOT(onZoom(qreal)));
}

void TestView::onZoom(qreal factor) {
   qDebug()<<"factor:"<<factor;
   item->setScale(1+factor);
}
// eof
