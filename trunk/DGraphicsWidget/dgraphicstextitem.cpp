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
    QString fmt("<html><head><body>%1</body></head></html>");

    mTextItem = new QGraphicsTextItem(text,this);
    mTextItem->setHtml(fmt.arg(text)); //test
}

void DGraphicsTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setBrush(QBrush(QColor(Qt::red)));
//    painter->drawRect(mTextItem->boundingRect().toRect());
}
