#ifndef ANALOGHAND_H
#define ANALOGHAND_H
#include <QGraphicsTextItem>

#include <QGraphicsLineItem>
#include <QGraphicsWidget>
class analoghand : public QGraphicsWidget

{
    Q_OBJECT;
    //Q_PROPERTY(qreal rotation READ rotation WRITE setRotation);
public:
    analoghand(QLine aLine, QGraphicsItem* aParent = 0);

    // Properties
    //qreal rotation() const{return QGraphicsLineItem::rotation();}
    //void setRotation(qreal aRotation);

    //void setTargetAngle(qreal aTargetAngle){iTargetAngle = aTargetAngle;}

//Q_SIGNALS:
    //void rotatedToTargetAngle();

private:
    qreal iTargetAngle;
    QGraphicsLineItem* iLineItem;
};

#endif // ANALOGHAND_H
