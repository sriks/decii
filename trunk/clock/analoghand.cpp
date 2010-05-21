#include <QLine>
#include "analoghand.h"

AnalogHand::AnalogHand(QLine aLine, QPen aPen, QGraphicsItem* aParent)
{
    iLineItem = new QGraphicsLineItem(this);
    iLineItem->setLine(aLine);
    iLineItem->setPen(aPen);
}

// eof
