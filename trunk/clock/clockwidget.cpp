#include <QDebug>
#include <QState>
#include <QStateMachine>
#include <QSignalTransition>
#include <QGraphicsTextItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QDateTime>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsBlurEffect>
#include <QDesktopWidget>
#include <QGraphicsProxyWidget>
#include <QLinearGradient>
#include <QVBoxLayout>
#include <QMetaObject>
#include "clockwidget.h"

//#define ALLOW_SHADOW_EFFECT

// No z value beyond this shd be allowed
const int KHoldingScrewZValue(5);
const int KSecondsHandZValue(KHoldingScrewZValue-1);
const int KMinutesHansZValue(KSecondsHandZValue-1);
const int KHoursHandZValue(KMinutesHansZValue-1);
const int KClockFrameZValue(KHoursHandZValue-1);
const int KBaseRectZValue(KClockFrameZValue);
const int KLongestHandLength(45);

ClockWidget::ClockWidget(QObject *parent)
{
iScene = new QGraphicsScene(this);
iScene->setSceneRect(rect());
setScene(iScene);
setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
QPalette palette = this->palette();
palette.setBrush(QPalette::Base,QBrush(QColor(0,0,0,128)));
setPalette(palette);

// Make this view transparent
setWindowFlags(Qt::FramelessWindowHint);
setAttribute(Qt::WA_TranslucentBackground,true);

// Create Analog Hands
iSecondsHand = new AnalogHand(QLine(0,5,0,-(KLongestHandLength-5)),
                              QPen(QBrush(QColor(Qt::red)),1,Qt::SolidLine,Qt::RoundCap));
iSecondsHand->setZValue(KSecondsHandZValue);
iSecondsHand->setOpacity(0.7);
iMinutesHand = new AnalogHand(QLine(0,5,0,-KLongestHandLength),
                              QPen(QBrush(QColor(Qt::green)),2,Qt::SolidLine,Qt::RoundCap));
iMinutesHand->setZValue(KMinutesHansZValue);
iMinutesHand->setOpacity(0.7);
iHoursHand = new AnalogHand(QLine(0,5,0,-(KLongestHandLength-15)),
                            QPen(QBrush(QColor(Qt::green)),3,Qt::SolidLine,Qt::RoundCap));
iHoursHand->setZValue(KHoursHandZValue);
iHoursHand->setOpacity(0.7);

QRect screwCircle(QPoint(width()/2,height()/2),QSize(10,10));
screwCircle.moveCenter(QPoint(width()/2,height()/2));
QGraphicsEllipseItem* holdingScrew = new QGraphicsEllipseItem(screwCircle);
holdingScrew->setBrush(QBrush(QColor(Qt::black)));
holdingScrew->setZValue(KHoldingScrewZValue);

// Create clock frame and place it at center
QRect circle(QPoint(width()/2,height()/2),QSize(250,250));
circle.moveCenter(QPoint(width()/2,height()/2));
QGraphicsEllipseItem* frameCircle = new QGraphicsEllipseItem(circle);
frameCircle->setPen(QPen(QBrush(QColor(Qt::green)),3));
frameCircle->setBrush(QBrush(QColor(Qt::black)));
frameCircle->setOpacity(0.9);
frameCircle->setZValue(KClockFrameZValue);
frameCircle->setGraphicsEffect(new QGraphicsBlurEffect);

QGraphicsTextItem* clocktext = new QGraphicsTextItem(tr("Qt Clock"));
clocktext->setDefaultTextColor(QColor(Qt::green));
clocktext->setPos(width()/2 - clocktext->boundingRect().width()/2,(height()/2)-60);
clocktext->setZValue(KClockFrameZValue);

#ifdef ALLOW_SHADOW_EFFECT
const int shadowBlurRadius(10);
const int shadowOffset(10);
QColor shadowColor(Qt::lightGray);
QGraphicsDropShadowEffect* secShadow = new QGraphicsDropShadowEffect(this);
secShadow->setBlurRadius(shadowBlurRadius);
secShadow->setOffset(shadowOffset);
secShadow->setColor(shadowColor);
iSecondsHand->setGraphicsEffect(secShadow);
QGraphicsDropShadowEffect* minShadow = new QGraphicsDropShadowEffect(this);
minShadow->setBlurRadius(shadowBlurRadius);
minShadow->setOffset(shadowOffset);
minShadow->setColor(shadowColor);
iMinutesHand->setGraphicsEffect(minShadow);
QGraphicsDropShadowEffect* hrShadow = new QGraphicsDropShadowEffect(this);
hrShadow->setBlurRadius(shadowBlurRadius);
hrShadow->setOffset(shadowOffset);
hrShadow->setColor(shadowColor);
iHoursHand->setGraphicsEffect(hrShadow);
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
QPen markingPen(QBrush(Qt::green),3,Qt::SolidLine,Qt::RoundCap);
for(int i=0;i<12;i++)
{
    QGraphicsLineItem* minuteMarking = new QGraphicsLineItem(
                                           QLine(0,-markingStart,0,-(markingStart+markingLength)));
    minuteMarking->setTransform(transform);
    minuteMarking->setPen(markingPen);
    minuteMarking->setRotation(i*30);
#ifdef BLUR_MARKINGS
    QGraphicsBlurEffect* blurEffect = new QGraphicsBlurEffect;
    blurEffect->setBlurRadius(1);
    minuteMarking->setGraphicsEffect(blurEffect);
#endif
    minuteMarking->setOpacity(0.7);
    minuteMarking->setZValue(KHoursHandZValue);
    iScene->addItem(minuteMarking);
}

// Add items to scene
iScene->addItem(iSecondsHand);
iScene->addItem(iMinutesHand);
iScene->addItem(iHoursHand);
iScene->addItem(frameCircle);
iScene->addItem(holdingScrew);
iScene->addItem(clocktext);

// Create animations
iStartupAnimation = new QParallelAnimationGroup(this);
iSecHandAnimation = new QPropertyAnimation(iSecondsHand,"rotation",this);
iSecHandAnimation->setEndValue(360);

iMinHandAnimation = new QPropertyAnimation(iMinutesHand,"rotation");
iMinHandAnimation->setDuration(1*1000);
iMinHandAnimation->setEasingCurve(QEasingCurve::OutQuad);

iHourHandAnimation = new QPropertyAnimation(iHoursHand,"rotation");
iHourHandAnimation->setDuration(1*1000);
iHourHandAnimation->setEasingCurve(QEasingCurve::OutQuad);

// When startup animation is finished, it should start clock ticking
connect(iStartupAnimation,SIGNAL(finished()),this, SLOT(on_secondshand_entered()));
connect(iSecHandAnimation,SIGNAL(finished()),this,SLOT(on_secondshand_exited()));
// connect slots to signals
QMetaObject::connectSlotsByName(this);

#ifndef Q_OS_SYMBIAN
setTransform(transform);
#endif

// sync with system time and start clock
on_sync_entered();
}

ClockWidget::~ClockWidget()
{
// left blank
}


void ClockWidget::on_sync_entered()
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
    iSecHandAnimation->start(QAbstractAnimation::KeepWhenStopped);
    }
}

void ClockWidget::on_secondshand_exited()
{
qDebug()<<"on_secondshand_exited()";
if(iSecHandAnimation)
{
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
    if(0 == int(iMinutesHand->rotation())%72)
    {
        iHourHandAnimation->setStartValue(iHoursHand->rotation());
        iHourHandAnimation->setEndValue(iHoursHand->rotation()+6);
        iHourHandAnimation->start();
    }
}

}


void ClockWidget::handleInterruption()
{
// Lock screen
#ifdef Q_OS_WIN32
    system("rundll32.exe user32.dll, LockWorkStation");
#endif
}



// eof
