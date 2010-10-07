#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include "dpageturneffect.h"
#include "dpagecurl.h"

const int KAnimationFrameTime = 100;

DPageTurnEffect::DPageTurnEffect(QObject* parent)
{
//    if(parent)
//    {
//        setParent(parent);
//    }
    mPageCurl = new DPageCurl;
    mPageCurl->setParent(this);
    QGraphicsScene* scene = new QGraphicsScene(this);
    setScene(scene);
    mPagePixmap = new QGraphicsPixmapItem;
    scene->addItem(mPagePixmap);
    mPagePixmap->setPos(scene->sceneRect().topLeft());
    mCurlPixmap = new QGraphicsPixmapItem;
    scene->addItem(mCurlPixmap);
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    mCurlPixmap->setGraphicsEffect(shadowEffect);
    makeTransparent();
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
    mTimerId = startTimer(KAnimationFrameTime);
    qDebug()<<"timerid:"<<mTimerId;
}


void DPageTurnEffect::timerEvent(QTimerEvent *event)
{
qDebug()<<__PRETTY_FUNCTION__;
    // Set page and curl
    QPixmap page = mPageCurl->nextPageCut();
    if(!page.isNull())
    {
//        mPagePixmap->setVisible(false);
        mPagePixmap->setPixmap(page);
    }

    QPixmap curl = mPageCurl->nextCurlCut();
    if(!curl.isNull())
    {
//        mCurlPixmap->setVisible(false);
        mCurlPixmap->setPos(scene()->width()-curl.width(),0);
        mCurlPixmap->setPixmap(curl);
//        mCurlPixmap->setVisible(true);
//        mPagePixmap->setVisible(true);
    }

    if(page.isNull() || curl.isNull())
    {
        killTimer(mTimerId);
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
