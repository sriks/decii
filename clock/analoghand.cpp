#include <QPen>

#include "analoghand.h"

analoghand::analoghand(QLine aLine, QGraphicsItem* aParent)
           //:QGraphicsLineItem(aParent)

{
    iLineItem = new QGraphicsLineItem(this);
    iLineItem->setLine(aLine);
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(5);
    pen.setCapStyle(Qt::RoundCap);
    iLineItem->setPen(pen);
}

/*analoghand::setRotation(qreal aRotation)
{
    QGraphicsLineItem::setRotation(aRotation);

    // emit signal if condition is satified
}*/
