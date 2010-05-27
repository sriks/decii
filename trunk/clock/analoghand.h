#ifndef ANALOGHAND_H
#define ANALOGHAND_H


/*
 This class holds a generic anoalog hands. Properties are
 inherited from graphicswidget for animation of hand movement.
 */

#include <QGraphicsLineItem>
#include <QGraphicsWidget>
#include <QPen>

class QLine;
class AnalogHand : public QGraphicsWidget
{
    Q_OBJECT;

public:
    AnalogHand(QLine aLine, QPen aPen = QPen(),QGraphicsItem* aParent = 0);

private:
    QGraphicsLineItem* iLineItem;
};

#endif // ANALOGHAND_H
