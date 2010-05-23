
#define TEST_ANIMATION

#include <QDebug>
#include <QState>
#include <QStateMachine>
#include <QSignalTransition>
#include <QGraphicsTextItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QDateTime>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsColorizeEffect>

#include "clockwidget.h"

// No z value beyond this shd be allowed
const int KSecondsHandZValue(5);
const int KMinutesHansZValue(KSecondsHandZValue-1);
const int KHoursHandZValue(KMinutesHansZValue-1);
const int KLongestHandLength(45);

ClockWidget::ClockWidget(QObject *parent)
{
iScene = new QGraphicsScene(this);

iScene->setSceneRect(rect());
setScene(iScene);

// Create Analog Hands
QPen secPen(QColor(Qt::blue));
secPen.setCapStyle(Qt::RoundCap);
secPen.setWidthF(1);
iSecondsHand = new AnalogHand(QLine(0,5,0,-(KLongestHandLength-5)),secPen);
iSecondsHand->setZValue(KSecondsHandZValue);

QPen minPen;
minPen.setColor(QColor(Qt::red));
minPen.setWidth(2);
minPen.setCapStyle(Qt::RoundCap);
iMinutesHand = new AnalogHand(QLine(0,5,0,-KLongestHandLength),minPen);
iMinutesHand->setParent(this);
iMinutesHand->setZValue(KMinutesHansZValue);
iMinutesHand->setOpacity(0.7);

minPen.setColor(Qt::black);
minPen.setWidth(2);
iHoursHand = new AnalogHand(QLine(0,5,0,-(KLongestHandLength-15)),minPen);
iHoursHand->setZValue(KHoursHandZValue);
setRenderHint(QPainter::Antialiasing,true);

const int clockFrameZValue(0);
QRect circle(QPoint(width()/2,height()/2),QSize(250,250));
circle.moveCenter(QPoint(width()/2,height()/2));
QGraphicsEllipseItem* frameCircle = new QGraphicsEllipseItem(circle);
frameCircle->setOpacity(0.6);
frameCircle->setBrush(QBrush(QColor(Qt::blue)));
frameCircle->setZValue(clockFrameZValue);

circle.setSize(QSize(circle.width()-50,circle.height()-50));
circle.moveCenter(QPoint(width()/2,height()/2));
QGraphicsEllipseItem* clockFace = new QGraphicsEllipseItem(circle);
clockFace->setBrush(QBrush(QColor(Qt::black)));
clockFace->setOpacity(0.6);
clockFace->setZValue(clockFrameZValue+1);


#ifdef ALLOW_SHADOW_EFFECT
const int shadowBlurRadius(10);
const int shadowOffset(10);
QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
shadow->setBlurRadius(shadowBlurRadius);
shadow->setOffset(shadowOffset);
QGraphicsDropShadowEffect* minShadow = new QGraphicsDropShadowEffect(this);
minShadow->setBlurRadius(shadowBlurRadius);
minShadow->setOffset(shadowOffset);
iMinutesHand->setGraphicsEffect(minShadow);
iSecondsHand->setGraphicsEffect(shadow);
#endif

// Apply transformation
int side = qMin(width(),height());
QTransform transform;
transform = transform.translate(width()/2,height()/2); //center of widget
#ifdef Q_OS_SYMBIAN
transform = transform.scale(side/400,side/400);
#else
transform = transform.scale(side/200,side/200);
#endif
iSecondsHand->setTransform(transform);
iMinutesHand->setTransform(transform);
iHoursHand->setTransform(transform);

// Create minute markings
const int markingStart = KLongestHandLength + 5;
const int markingLength = 5;
QPen markingPen(QBrush(Qt::black),3);
for(int i=0;i<12;i++)
{
    /*AnalogHand* minuteMarking = new AnalogHand(QLine(0,-markingStart,0,-(markingStart+markingLength)),
                                               markingPen);
    minuteMarking->setParent(this);
    minuteMarking->setTransform(transform);
    //minuteMarking->setLine(0,-markingStart,0,-(markingStart+markingLength));
    markingPen.setColor(Qt::red);
    minuteMarking->lineItem().setPen(markingPen);

    minuteMarking->setRotation(i*30);
    qDebug()<<i<<" minute marking angle "<<minuteMarking->rotation();
    minuteMarking->setOpacity(0.7);
    minuteMarking->setZValue(KHoursHandZValue);
    iScene->addItem(minuteMarking);*/


    QGraphicsLineItem* minuteMarking = new QGraphicsLineItem(
                                           QLine(0,-markingStart,0,-(markingStart+markingLength)));
    //minuteMarking->s`setParent(this);
    minuteMarking->setTransform(transform);
    minuteMarking->setPen(markingPen);

    minuteMarking->setRotation(i*30);
    qDebug()<<i<<" minute marking angle "<<minuteMarking->rotation();
    minuteMarking->setOpacity(0.7);
    minuteMarking->setZValue(KHoursHandZValue);
    iScene->addItem(minuteMarking);
}

iScene->addItem(iSecondsHand);
iScene->addItem(iMinutesHand);
iScene->addItem(iHoursHand);
iScene->addItem(frameCircle);
iScene->addItem(clockFace);


// Create states
iStateMachine = new QStateMachine(this);
iParentState = new QState(iStateMachine);
iSyncState = new QState(iParentState);
iSyncState->setObjectName("syncstate");
iSecondsHandState = new QState(iParentState);
iSecondsHandState->setObjectName("secondshand");
iMinutesHandState = new QState(iParentState);
iMinutesHandState->setObjectName("minuteshand");
iHoursHandState = new QState(iParentState);

iStartupAnimation = new QParallelAnimationGroup(this);
iSecHandAnimation = new QPropertyAnimation(iSecondsHand,"rotation",this);
iSecHandAnimation->setEndValue(360);

iMinHandAnimation = new QPropertyAnimation(iMinutesHand,"rotation");
iMinHandAnimation->setDuration(1*1000);
iMinHandAnimation->setEasingCurve(QEasingCurve::OutQuad);

iHourHandAnimation = new QPropertyAnimation(iHoursHand,"rotation");
iHourHandAnimation->setDuration(1*1000);
iHourHandAnimation->setEasingCurve(QEasingCurve::OutQuad);

iSyncState->addTransition(iStartupAnimation,SIGNAL(finished()),iSecondsHandState);
iSecondsHandState->addTransition(iSecHandAnimation,SIGNAL(finished()),iSecondsHandState);
iParentState->setInitialState(iSyncState);
iStateMachine->setInitialState(iParentState);
//iStateMachine->setGlobalRestorePolicy(QStateMachine::RestoreProperties);
QMetaObject::connectSlotsByName(this);
iStateMachine->start();
this->setTransform(transform);//scale(width()/100,height()/100);

}

ClockWidget::~ClockWidget()
{

}


void ClockWidget::on_syncstate_entered()
{
    qDebug()<<"ClockWidget::on_syncstate_entered()->";

    // Synchronize hands angle with system time
    QDateTime systemDateTime = QDateTime::currentDateTime();
    qDebug()<<"current time "<<systemDateTime.time().toString();

    int startupAnimDuration(1*1000);
    QEasingCurve::Type easingCurve = QEasingCurve::OutQuad;
    QPropertyAnimation* secAnim = new QPropertyAnimation(iSecondsHand,"rotation",this);
    secAnim->setDuration(startupAnimDuration);
    secAnim->setEndValue(systemDateTime.time().second()*6);
    secAnim->setEasingCurve(easingCurve);
    iSecHandAnimation->setStartValue(secAnim->endValue());
    qDebug()<<"iSecHandAnimation->startValue():"<<iSecHandAnimation->startValue().toInt();

    QPropertyAnimation* minAnim = new QPropertyAnimation(iMinutesHand,"rotation",this);
    minAnim->setDuration(startupAnimDuration);
    minAnim->setEndValue(systemDateTime.time().minute()*6);
    minAnim->setEasingCurve(easingCurve);

    QPropertyAnimation* hrAnim = new QPropertyAnimation(iHoursHand,"rotation",this);
    hrAnim->setDuration(startupAnimDuration);
    hrAnim->setEndValue((systemDateTime.time().hour()*30) + (systemDateTime.time().minute()/12)*6);
    hrAnim->setEasingCurve(easingCurve);
    qDebug()<<"hr:"<<iHoursHand->rotation()<<" min:"<<iMinutesHand->rotation()<<" sec:"<<iSecondsHand->rotation();

    //int newDuration = ((360-iSecondsHand->rotation())*60)/360;
    int newDuration = ((360- (iSecHandAnimation->startValue().toInt()))*60)/360;
    qDebug()<<"newDuration :"<<newDuration;
    iSecHandAnimation->setDuration(newDuration*1000);

    iStartupAnimation->addAnimation(secAnim);
    iStartupAnimation->addAnimation(minAnim);
    iStartupAnimation->addAnimation(hrAnim);
    iStartupAnimation->start();
    qDebug()<<"ClockWidget::on_syncstate_entered()<-";
}

void ClockWidget::on_secondshand_entered()
{
qDebug()<<"ClockWidget::on_secondshand_entered()";
    if(iSecHandAnimation)
    {
        qDebug()<<"iSecHandAnimation not null";

    iSecHandAnimation->start(QAbstractAnimation::KeepWhenStopped);
    }

    else
    {
        qDebug()<<"iSecHandAnimation NULL";
    }
}

void ClockWidget::on_secondshand_exited()
{
qDebug()<<"on_secondshand_exited()";
if(iSecHandAnimation)
{
    qDebug()<<"iSecHandAnimation not null";
    // Reset start value and start again
    iSecHandAnimation->setStartValue(0);
    iSecHandAnimation->setEndValue(360);
    iSecHandAnimation->setDuration(60*1000); // 60 sec
    iSecHandAnimation->start(QAbstractAnimation::KeepWhenStopped);

    // Animate minutes hand
    iMinHandAnimation->setStartValue(iMinutesHand->rotation());
    iMinHandAnimation->setEndValue(iMinutesHand->rotation()+6);
    iMinHandAnimation->start();

    // Animate hours hand
    if(0 == iMinutesHand->rotation()/12)
    {
        iHourHandAnimation->setStartValue(iHoursHand->rotation());
        iHourHandAnimation->setEndValue(iHoursHand->rotation()+6);
        iHourHandAnimation->start();
    }
}

else
{
    qDebug()<<"iSecHandAnimation NULL";
}

}

void ClockWidget::on_minuteshand_exited()
{
qDebug()<<"ClockWidget::on_minuteshand_exited()";
}



// eof
