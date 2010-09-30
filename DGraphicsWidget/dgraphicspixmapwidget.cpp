#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "dgraphicspixmapwidget.h"

DGraphicsPixmapWidget::DGraphicsPixmapWidget(QPixmap pixmap,QGraphicsItem* parent)
    :QGraphicsWidget(parent)
{
    mPixmapItem = new QGraphicsPixmapItem(pixmap,this);
}

void DGraphicsPixmapWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    setPreferredSize(mPixmapItem->boundingRect().size());
//    painter->setPen(QPen(QColor(Qt::red)));
//    painter->drawRect(rect());
}

void DGraphicsPixmapWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<__FUNCTION__;
    event->accept();
    emit triggered();
}
