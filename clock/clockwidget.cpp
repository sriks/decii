
#include <QDebug>
#include <QState>
#include <QStateMachine>
#include <QGraphicsTextItem>
#include <QGraphicsLinearLayout>
#include <QGraphicsAnchorLayout>
#include <QPropertyAnimation>

#include <QGraphicsScene>

#include "clockwidget.h"

ClockWidget::ClockWidget(QGraphicsItem *parent)
{
// Create Analog Hands
iSecondsHand = new AnalogHand(QLine(0,0,0,30));
iMinutesHand = new AnalogHand(QLine(0,0,0,50),QPen(QColor(Qt::red)));
iMinutesHand->setParent(this);
QGraphicsTextItem textitem("text item",this,this->scene());

QGraphicsLinearLayout* layout =  new QGraphicsLinearLayout(Qt::Vertical);
layout->addItem(iSecondsHand);
layout->addItem(iMinutesHand);
this->setLayout(layout);

/*QGraphicsAnchorLayout* iLayout = new QGraphicsAnchorLayout(this);
iLayout->addAnchor(iSecondsHand,Qt::AnchorBottom,iMinutesHand,Qt::AnchorBottom);
//iLayout->addAnchor(iMinutesHand,Qt::AnchorBottom,iSecondsHand,Qt::AnchorBottom);*/

QTransform transform;
transform = transform.translate(this->rect().center().toPoint().x(),this->rect().center().toPoint().y());
iSecondsHand->setTransform(transform);
iMinutesHand->setTransform(transform);

// add animations
QPropertyAnimation* anim = new QPropertyAnimation(iSecondsHand,"rotation");
anim->setDuration(60*1000); // 60 secs
anim->setStartValue(qreal(0));
anim->setEndValue(qreal(360));
anim->setEasingCurve(QEasingCurve::Linear);
anim->start(QAbstractAnimation::DeleteWhenStopped);

}

ClockWidget::~ClockWidget()
{

}

// eof
