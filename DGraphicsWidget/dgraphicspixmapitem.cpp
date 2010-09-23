#include <QGraphicsPixmapItem>
#include "dgraphicspixmapitem.h"

DGraphicsPixmapItem::DGraphicsPixmapItem(QPixmap pixmap,QGraphicsItem* parent)
                    :QGraphicsWidget(parent)
{
    mPixmapItem = new QGraphicsPixmapItem(pixmap,this);
}
