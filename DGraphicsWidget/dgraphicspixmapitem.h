#ifndef DGRAPHICSPIXMAPITEM_H
#define DGRAPHICSPIXMAPITEM_H
#include <QObject>
#include <QGraphicsWidget>

class QGraphicsItem;
class QGraphicsPixmapItem;
class QPixmap;
class DGraphicsPixmapItem : public QGraphicsWidget
{
    Q_OBJECT
public:
    DGraphicsPixmapItem(QPixmap pixmap,QGraphicsItem* parent = 0);
    QGraphicsPixmapItem* pixmapItem(){return mPixmapItem;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void triggered();
private:
    QGraphicsPixmapItem* mPixmapItem;
};

#endif // DGRAPHICSPIXMAPITEM_H
