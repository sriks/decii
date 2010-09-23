#ifndef DGRAPHICSPIXMAPITEM_H
#define DGRAPHICSPIXMAPITEM_H

#include <QGraphicsWidget>
class QPixmap;
class QGraphicsPixmapItem;
class DGraphicsPixmapItem : public QGraphicsWidget
{
public:
    DGraphicsPixmapItem(QPixmap pixmap,QGraphicsItem* parent = 0);
    QGraphicsPixmapItem* pixmapItem(){return mPixmapItem;}
private:
    QGraphicsPixmapItem* mPixmapItem;
};

#endif // DGRAPHICSPIXMAPITEM_H
