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
    setOwnedByLayout(false);
}

DGraphicsPixmapWidget::~DGraphicsPixmapWidget()
{
qDebug()<<__PRETTY_FUNCTION__;
}

void DGraphicsPixmapWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
#ifdef DEBUG_PAINT
    painter->setPen(QPen(QColor(Qt::yellow)));
    painter->drawRect(rect());
#endif
}

void DGraphicsPixmapWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<__FUNCTION__;
    event->accept();
    emit triggered();
}

void DGraphicsPixmapWidget::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    qDebug()<<__PRETTY_FUNCTION__;
    resize(mPixmapItem->boundingRect().size());
}
// eof
