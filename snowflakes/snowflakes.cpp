#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QDebug>
#include "snowflakes.h"

const int KSnowflakesCreationInterval = 1000*5;
const int KAnimationDuration          = 1000*5;

Snowflakes::Snowflakes(QObject* aParent)
{
    QGraphicsScene* scene = new QGraphicsScene(this);
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHints(QPainter::Antialiasing);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Base,QBrush(QColor(0,0,0,128))); // transparent bkg
    setPalette(palette);
    // Make this view transparent
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);
    bool loaded = mSnowflakePixmap.load(":/images/snowflake.png");
    qDebug()<<loaded;
    show();
}

void Snowflakes::startSnowfall()
{
    mTimerId = startTimer(KSnowflakesCreationInterval);
}

void Snowflakes::stopSnowfall()
{
    killTimer(mTimerId);
}

void Snowflakes::timerEvent(QTimerEvent *aEvent)
{
    QPropertyAnimation* anim = new QPropertyAnimation(this);
    QGraphicsWidget* snowflake = createSnowflake();
    this->scene()->addItem(snowflake);
    anim->setTargetObject(snowflake);
    anim->targetObject()->setParent(anim); //to delete when anim is finished
    anim->setProperty("pos",QVariant(QPoint()));
    anim->setStartValue(QPoint(50,50));
    anim->setEndValue(QPoint(500,500));
    anim->setDuration(KAnimationDuration);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

QGraphicsWidget* Snowflakes::createSnowflake()
{
    QGraphicsWidget* snowflakeWidget = new QGraphicsWidget;
    QGraphicsPixmapItem* snowflake = new QGraphicsPixmapItem(snowflakeWidget);
    snowflake->setPixmap(mSnowflakePixmap);
    return snowflakeWidget;
}
