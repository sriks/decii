#include <QPainter>
#include "dgraphicstextitem.h"

//DGraphicsTextItem::DGraphicsTextItem(QGraphicsTextItem* textItem,
//                                     QGraphicsItem* parent)
//                  :QGraphicsWidget(parent),
//                   mTextItem(textItem)
//{
//mTextItem->setParent(this);
//}

DGraphicsTextItem::DGraphicsTextItem(QString text, QGraphicsItem* parent)
                  :QGraphicsWidget(parent)
{
    mTextItem = new QGraphicsTextItem(text,this);
}

void DGraphicsTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QBrush(QColor(Qt::red)));
    painter->drawRect(mTextItem->boundingRect().toRect());
}
