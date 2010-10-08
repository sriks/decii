#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include "dgraphicslayer.h"
#include "dpagecurl.h"

//#define DRAW_HERE
#define NO_GRAD
DGraphicsLayer::DGraphicsLayer(DPageCurl* curl)
               :mPageCurl(curl),
                mUsePageCurl(true)
{
}

void DGraphicsLayer::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
qDebug()<<__PRETTY_FUNCTION__;
qDebug()<<"new pos:"<<pos();


#ifdef DRAW_HERE
    int curlWidth = mPageCurl->hostWidgetSize().width() - pos().x();
    resize(curlWidth,curlWidth);
    QPainterPath curlPath = mPageCurl->nextCurlPath(rect());
#ifdef NO_GRAD
    QBrush curlBrush(Qt::gray);
    painter->fillPath(curlPath,curlBrush);
#else
    QLinearGradient gradient(0,0,size().width(),size().height());
    gradient.setSpread(QGradient::ReflectSpread);
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.8, Qt::lightGray);
    QBrush curlBrush(gradient);
    painter->fillPath(curlPath,curlBrush);
#endif

#else
    QPixmap pixmap;
    mPageCurl->nextPageCut(); // for test
    pixmap = mPageCurl->nextCurlCut__(pos());
    if(pixmap.isNull())
    {
        qDebug()<<"pixmap is null";
        return;
    }
    resize(pixmap.size());
    painter->drawPixmap(pixmap.rect(),pixmap);
#endif
}

QPointF DGraphicsLayer::curl()
{
    qDebug()<<__PRETTY_FUNCTION__;
    return pos();
}

void DGraphicsLayer::setCurl(QPointF curlPos)
{
    qDebug()<<__PRETTY_FUNCTION__;
    qDebug()<<"pos:"<<curlPos;
    setPos(curlPos);
    update();
}
