#include <QPainter>
#include "dgraphicstextwidget.h"

DGraphicsTextWidget::DGraphicsTextWidget(QString text, QGraphicsItem* parent)
                    :QGraphicsWidget(parent)
{
    mTextItem = new QGraphicsTextItem(text,this);
}

void DGraphicsTextWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(Qt::white)));
//    painter->drawRect(rect());
}
