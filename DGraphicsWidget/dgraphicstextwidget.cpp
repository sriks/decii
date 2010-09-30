#include <QPainter>
#include <QDebug>
#include "dgraphicstextwidget.h"

DGraphicsTextWidget::DGraphicsTextWidget(QString text, QGraphicsItem* parent)
                    :QGraphicsWidget(parent)
{
    mTextItem = new QGraphicsTextItem(text,this);
    setOwnedByLayout(false);
}

DGraphicsTextWidget::~DGraphicsTextWidget()
{
qDebug()<<__PRETTY_FUNCTION__;
}

void DGraphicsTextWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->setPen(QPen(QColor(Qt::white)));
//    painter->drawRect(rect());
}
