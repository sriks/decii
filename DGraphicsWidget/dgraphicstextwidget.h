#ifndef DGraphicsTextWidget_H
#define DGraphicsTextWidget_H

#include <QGraphicsWidget>
#include <QGraphicsTextItem>

class DGraphicsTextWidget: public QGraphicsWidget
{
public:
    DGraphicsTextWidget(QString text=QString(), QGraphicsItem* parent=0);
    ~DGraphicsTextWidget();
    QGraphicsTextItem* textItem(){return mTextItem;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void resizeEvent(QGraphicsSceneResizeEvent* event);

private:
    QGraphicsTextItem* mTextItem;
};

#endif // DGraphicsTextWidget_H
