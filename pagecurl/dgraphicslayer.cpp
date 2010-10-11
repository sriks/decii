#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "dgraphicslayer.h"
#include "dpagecurl.h"

#define DRAW_HERE
//#define NO_GRAD
DGraphicsLayer::DGraphicsLayer(DPageCurl* curl,bool isCurl)
               :mPageCurl(curl),
                mUsePageCurl(isCurl)
{
}

void DGraphicsLayer::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
qDebug()<<__PRETTY_FUNCTION__;
qDebug()<<"new pos:"<<pos();
//qDebug()<<"items rect:"<<rect()
//        <<"expsd rect:"<<option->exposedRect;

painter->setClipRect(option->exposedRect);
painter->setRenderHint(QPainter::Antialiasing);
if(isUsingPageCurl())
    {
//    qDebug()<<"Painting pagecurl from "<<pos();
//    #ifdef DRAW_HERE
//        int curlWidth = mPageCurl->hostWidgetSize().width() - pos().x();
//        resize(curlWidth,curlWidth);
//        QPainterPath curlPath = mPageCurl->nextCurlPath(rect());
//    #ifdef NO_GRAD
//        QBrush curlBrush(Qt::gray);
//    #else
//        QLinearGradient gradient(0,0,size().width(),size().height());
//        gradient.setSpread(QGradient::ReflectSpread);
//        gradient.setColorAt(0.0, Qt::white);
//        gradient.setColorAt(0.8, Qt::lightGray);
//        QBrush curlBrush(gradient);
//    #endif
//        painter->fillPath(curlPath,curlBrush);
//    #else
//        QPixmap pixmap;
//        mPageCurl->nextPageCut(); // for test
//        pixmap = mPageCurl->nextCurlCut__(pos());
//        if(pixmap.isNull())
//        {
//            qDebug()<<"pixmap is null";
//            return;
//        }
//        resize(pixmap.size());
//        painter->drawPixmap(pixmap.rect(),pixmap);
//    #endif
    }

    else
    {
//        qDebug()<<"Painting pagecut from "<<mPageCutPos;
//        QPainterPath entirePainterPath;
//        entirePainterPath.addRect(mPageCurl->hostWidgetRect());
//        int curlWidth = mPageCurl->hostWidgetSize().width() - mPageCutPos.x();
//        // Get the curl path to be ignored
//        QPainterPath curlPathToIgnore = mPageCurl->nextCurlPath(QRectF(mPageCutPos,
//                                                                       QSize(curlWidth,curlWidth)));
//        QPainterPath paintPath = entirePainterPath.subtracted(curlPathToIgnore);
//        qDebug()<<"painPath:"<<paintPath;
//        painter->setClipPath(paintPath);
//        painter->drawPixmap(rect().toRect(),mPageCurl->hostWidgetAsPixmap());
    }
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

QPointF DGraphicsLayer::pageCut()
{
    return mPageCutPos;
}

void DGraphicsLayer::setPageCut(QPointF pageCutPos)
{
    mPageCutPos = pageCutPos;
    update();
}
