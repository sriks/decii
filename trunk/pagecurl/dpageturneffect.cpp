#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDebug>
#include "dpageturneffect.h"
#include "dgraphicslayer.h"
#include "dpagecurl.h"

const int KAnimationFrameTime = 0;
#define TESTANIM
DPageTurnEffect::DPageTurnEffect(QObject* parent)
{
    mPageCurl = new DPageCurl;
    mPageCurl->setParent(this);
    QGraphicsScene* scene = new QGraphicsScene(this);
    setScene(scene);
    makeTransparent();
#ifdef TESTANIM
    mCurlLayer = new DGraphicsLayer(mPageCurl,true);
    mCurlLayer->usePageCurl(true);
    mCurlLayer->setObjectName("curllayer");
    mCurlLayer->setZValue(10);
    this->scene()->addItem(mCurlLayer);
//    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(mCurlLayer);
//    mCurlLayer->setGraphicsEffect(shadowEffect);

    mPageCutLayer = new DGraphicsLayer(mPageCurl,false);
    mPageCutLayer->setObjectName("pagecutlayer");
    this->scene()->addItem(mPageCutLayer);

    setCacheMode(QGraphicsView::CacheBackground);
#else

    // create page and curl
    mPagePixmap = new QGraphicsPixmapItem;
    scene->addItem(mPagePixmap);
    mPagePixmap->setPos(scene->sceneRect().topLeft());
    mCurlPixmap = new QGraphicsPixmapItem;
    scene->addItem(mCurlPixmap);
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    mCurlPixmap->setGraphicsEffect(shadowEffect);
#endif
}

void DPageTurnEffect::setWidget(QWidget* hostWidget)
{
    mPageCurl->setWidget(hostWidget);
    // resize to host widget
    QSize hostSize = mPageCurl->hostWidgetSize();
    scene()->setSceneRect(mPageCurl->hostWidgetRect());
    resize(hostSize);
}

void DPageTurnEffect::startEffect()
{
qDebug()<<__PRETTY_FUNCTION__;
    mPageCurl->hostWidget()->setVisible(false);
    setVisible(true);
    show();
#ifdef TESTANIM
    testAnimation();
#else
    mTimerId = startTimer(KAnimationFrameTime);
    qDebug()<<"timerid:"<<mTimerId;
#endif
}


void DPageTurnEffect::timerEvent(QTimerEvent *event)
{
qDebug()<<__PRETTY_FUNCTION__;
    // Set page and curl
    QPixmap page = mPageCurl->nextPageCut();
    if(!page.isNull())
    {
        mPagePixmap->setPixmap(page);
    }

    QPixmap curl = mPageCurl->nextCurlCut();
    if(!curl.isNull())
    {
        mCurlPixmap->setPos(scene()->width()-curl.width(),0);
        mCurlPixmap->setPixmap(curl);
    }

    if(page.isNull() || curl.isNull())
    {
        killTimer(mTimerId);
        qDebug()<<"curl end pos:"<<mCurlPixmap->pos();
        return;
    }
}

void DPageTurnEffect::makeTransparent()
{
    // prepare see through window
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setAttribute(Qt::WA_ContentsPropagated,true);

    // transparent background
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Base, Qt::transparent);
    setPalette(palette);
    setAttribute(Qt::WA_OpaquePaintEvent, false); // trasmit color
    setFrameStyle(QFrame::NoFrame);
}

void DPageTurnEffect::testAnimation()
{
qDebug()<<__PRETTY_FUNCTION__;

QSize hostSize = mPageCurl->hostWidgetSize();
scene()->setSceneRect(mPageCurl->hostWidgetRect());
resize(hostSize);

    mPageCutLayer->resize(mPageCurl->hostWidgetSize());

    int animtime(1200);
    QEasingCurve easingCurve(QEasingCurve::InOutCubic);

    // Create curl animation
    QPropertyAnimation* curlAnimation = new QPropertyAnimation(mCurlLayer,"curl");
    curlAnimation->setDuration(animtime);
    QPoint startPoint;
    startPoint.setX(mPageCurl->hostWidgetSize().width()-mPageCurl->nextCurlWidth());
    startPoint.setY(0);
    curlAnimation->setStartValue(startPoint);
//    qDebug()<<"startpoint:"<<startPoint;
//    qDebug()<<"Min of w,h:"<<qMin(mPageCurl->hostWidgetSize().width(),
//                                  mPageCurl->hostWidgetSize().height());
    QPoint endPoint;
    endPoint.setX(mPageCurl->hostWidgetSize().width() - qMin(mPageCurl->hostWidgetSize().width(),
                                                             mPageCurl->hostWidgetSize().height()));
    qDebug()<<"endpoint:"<<endPoint;
    curlAnimation->setEndValue(endPoint);
    curlAnimation->setEasingCurve(easingCurve);

    // Create pagecut animation
    QPropertyAnimation* pageCutAnimation = new QPropertyAnimation(mPageCutLayer,"pageCut");
    pageCutAnimation->setDuration(animtime);
    pageCutAnimation->setStartValue(startPoint);
    pageCutAnimation->setEndValue(endPoint);
    pageCutAnimation->setEasingCurve(easingCurve);
    QParallelAnimationGroup* parallelAnimGroup = new QParallelAnimationGroup(this);
    parallelAnimGroup->addAnimation(curlAnimation);
    parallelAnimGroup->addAnimation(pageCutAnimation);
    parallelAnimGroup->start(QAbstractAnimation::DeleteWhenStopped);
    mPageCutLayer->show();
    mCurlLayer->show();
}

// eof
