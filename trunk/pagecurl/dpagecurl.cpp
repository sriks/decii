#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QWidget>
#include <QPainterPath>
#include <QPolygonF>
#include <QRegion>
#include "dpagecurl.h"

const QString KImageSource("source.jpg");
const QString KImageCurlCut("curlcut");
const QString KImagePageCut("pagecut");
const QString KImageCurlTmp("curltmp");
const QString KImageDotFormat(".png"); // png supports transparent backgrounds
const int KImageQualityHigh(100);

DPageCurl::DPageCurl(QGraphicsItem *parent)
    : QGraphicsWidget(parent),
      mIsHeightMin(true)
{

}

DPageCurl::~DPageCurl()
{

}

void DPageCurl::setWidget(QWidget* widget)
{
    mSourceWidget = widget;
    if(mSourceWidget)
    {
        mSourceImg = captureOriginal(mSourceWidget);

        // Identify shortest size
        QSize widgetsize = widget->size();
        if(widgetsize.height() > widgetsize.width())
        {
            mIsHeightMin = false;
            mPageLength = widgetsize.width();
        }
        else
        {
            mIsHeightMin = true;
            mPageLength = widgetsize.height();
        }

        // Calculate curl factor
        const int KPercentageOfImageToCurl(10);
        mCurlFactor = (KPercentageOfImageToCurl*mPageLength)/100;
        mPageCurlStartingPoint = mSourceWidget->rect().topRight();
        mPageCutStartingPoint = mSourceWidget->rect().topLeft();
        mPageCutCount = 1;
        qDebug()<<"curlfactor:"<<mCurlFactor<<"\npagelength:"<<mPageLength<<"\nisheightmin:"<<mIsHeightMin<<
                  "\npagecutstartingpoint:"<<mPageCutStartingPoint;


    }
}

// if next page cut is not valid returns a null pixmap
QPixmap DPageCurl::nextPageCut()
{
    mSourceWidget->setVisible(false);
    int curlLength = mPageCutCount*mCurlFactor;
    if(curlLength < mPageLength)
    {
      QRegion pageCutRegion = grabPageCut(mSourceImg,curlLength,curlLength);

      // extract this region from widget
      QImage img(mSourceWidget->size(),QImage::Format_ARGB32_Premultiplied);
      QPainter painter(&img);
      mSourceWidget->render(&painter,QPoint(),pageCutRegion);
#ifdef QT_DEBUG
      img.save(KImagePageCut+QString().setNum(mPageCutCount),
               "png",KImageQualityHigh); // for debug
      qDebug()<<"Pagecut count:"<<mPageCutCount<<"\nCurl length:"<<curlLength;
#endif
       ++mPageCutCount;
      // return valid image
      return QPixmap::fromImage(img);
    }

// return null pixmap
return QPixmap();
}

QPixmap DPageCurl::nextCurlCut()
{
    int curlCount = mPageCutCount - 1;
    QImage img(curlCount*mCurlFactor,curlCount*mCurlFactor,QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);

    QVector<QPoint> curlPoints;
    QRect curlRect = img.rect();
    painter.fillRect(curlRect,Qt::transparent);
    curlPoints.append(curlRect.bottomLeft());
    curlPoints.append(curlRect.bottomRight());

    QPainterPath curlPath(curlRect.topLeft());
    for(int i=0;i<curlPoints.count();i++)
    {
        curlPath.lineTo(curlPoints.value(i));
    }
    curlPath.closeSubpath();

    QBrush curlBrush(QColor(Qt::blue));
    painter.fillPath(curlPath,curlBrush);
    return QPixmap::fromImage(img);
}

QImage DPageCurl::captureOriginal(QWidget* widget)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QImage img(widget->size(),QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);
    widget->render(&painter);
    return img;
}


QRegion DPageCurl::grabPageCut(QImage image,int width,int height)
{
    qDebug()<<__PRETTY_FUNCTION__;
    //TODO: I Think argument image can be directly used.
    QImage tmpImage(image);
    QPainter painter(&tmpImage);

    // Create page points through which we cut the page
    QVector<QPointF> pageCutPoints;

    // This is fpr pagecut starting from topright
    pageCutPoints.append(QPointF(mPageCurlStartingPoint.x()-width,mPageCurlStartingPoint.y())); // point A
    pageCutPoints.append(QPointF(mPageCurlStartingPoint.x(),mPageCurlStartingPoint.y()+height)); // point B
    pageCutPoints.append(image.rect().bottomRight());
    pageCutPoints.append(image.rect().bottomLeft());

    // add to path
    QPainterPath pageCutPath(mPageCutStartingPoint);
    for(int i=0;i<pageCutPoints.count();i++)
    {
        pageCutPath.lineTo(pageCutPoints.value(i));
    }
    // close the path with starting point
    pageCutPath.closeSubpath();
    qDebug()<<"cutpoints:"<<pageCutPoints;
    qDebug()<<"curpath:"<<pageCutPath;

    painter.setClipPath(pageCutPath,Qt::ReplaceClip);
    return painter.clipRegion();
}

void DPageCurl::doCurl(QWidget* widget,int width,int height)
{
//    qDebug()<<__PRETTY_FUNCTION__;
//    widget->setVisible(false);
//    QImage sourceimg = captureOriginal(widget);
//    qDebug()<<"Source Image Saved:"<<sourceimg.save(KImageSource);

//    QRegion pageCutRegion = grabPageCut(sourceimg,width,height);
//    QImage img(widget->size(),QImage::Format_ARGB32_Premultiplied);
//    QPainter painter(&img);
//    widget->render(&painter,QPoint(),pageCutRegion);
//    img.save(KImagePageCut,"png",KImageQualityHigh);
}

//eof
