#include <QPainter>
#include <QDebug>
#include <QWidget>
#include <QTextDocument>
#include <QGraphicsSceneResizeEvent>
#include "dgraphicstextwidget.h"
//#define DEBUG_PAINT
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
#ifdef DEBUG_PAINT
    painter->setPen(QPen(QColor(Qt::red)));
    painter->drawRect(this->rect());
#endif
}

void DGraphicsTextWidget::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    qDebug()<<__PRETTY_FUNCTION__;
    qDebug()<<"docsize:"<<mTextItem->document()->size();
    mTextItem->setTextWidth(rect().width());
    QSizeF newSize = mTextItem->document()->size();
    newSize.setHeight(newSize.height() + ((10*newSize.height())/100) ); // % of height
    resize(newSize);
}
