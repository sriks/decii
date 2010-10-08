#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsWidget>
#include <QPropertyAnimation>
#include <QDebug>
#include "dpageturneffect.h"
#include "dgraphicslayer.h"
#include "dpagecurl.h"

const int KAnimationFrameTime = 100;
#define TESTANIM
DPageTurnEffect::DPageTurnEffect(QObject* parent)
{
    mPageCurl = new DPageCurl;
    mPageCurl->setParent(this);
    QGraphicsScene* scene = new QGraphicsScene(this);
    setScene(scene);
    makeTransparent();
#ifdef TESTANIM
    mCurlLayer = new DGraphicsLayer(mPageCurl);
    mCurlLayer->setZValue(10);
    scene->addItem(mCurlLayer);
//    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(mCurlLayer);
//    mCurlLayer->setGraphicsEffect(shadowEffect);
    mPagePixmap = new QGraphicsPixmapItem;
    scene->addItem(mPagePixmap);
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
        mPagePixmap->setVisible(false);
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

    mPagePixmap->setPixmap(mPageCurl->nextPageCut()); // test

    mCurlLayer->show();
    int animtime(2000);
    // Create rotation animation
    QPropertyAnimation* curlAnim = new QPropertyAnimation(mCurlLayer,"curl");
    curlAnim->setDuration(animtime);
    QPoint startPoint;
    qDebug()<<"hostsize:"<<mPageCurl->hostWidgetSize()
            <<"\nnextcurlwidth:"<<mPageCurl->nextCurlWidth();
    startPoint.setX(mPageCurl->hostWidgetSize().width()-mPageCurl->nextCurlWidth());
    startPoint.setY(0);
    curlAnim->setStartValue(startPoint);
    qDebug()<<"startpoint:"<<startPoint;
    qDebug()<<"Min of w,h:"<<qMin(mPageCurl->hostWidgetSize().width(),
                                  mPageCurl->hostWidgetSize().height());
    QPoint endPoint;
    endPoint.setX(mPageCurl->hostWidgetSize().width() - qMin(mPageCurl->hostWidgetSize().width(),
                                                             mPageCurl->hostWidgetSize().height()));
    qDebug()<<"endpoint:"<<endPoint;
    curlAnim->setEndValue(endPoint);
    curlAnim->setEasingCurve(QEasingCurve::Linear);
    curlAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

// eof
