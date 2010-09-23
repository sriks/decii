#ifndef DGRAPHICSTEXTITEM_H
#define DGRAPHICSTEXTITEM_H

#include <QGraphicsWidget>
#include <QGraphicsTextItem>

class DGraphicsTextItem : public QGraphicsWidget
{
public:
    //DGraphicsTextItem(QGraphicsTextItem* textItem, QGraphicsItem* parent=0);
    DGraphicsTextItem(QString text=QString(), QGraphicsItem* parent=0);
    QGraphicsTextItem* textItem(){return mTextItem;}
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
    QGraphicsTextItem* mTextItem;
};

#endif // DGRAPHICSTEXTITEM_H
