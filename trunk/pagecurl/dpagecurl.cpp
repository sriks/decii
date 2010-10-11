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

DPageCurl::DPageCurl(QObject *parent)
    : QObject(parent),
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
        const int KPercentageOfImageToCurl(1);
        mCurlFactor = (KPercentageOfImageToCurl*mPageLength)/100;
        mPageCurlStartingPoint = mSourceWidget->rect().topRight();
        mPageCutStartingPoint = mSourceWidget->rect().topLeft();
        mPageCutCount = 1;
        qDebug()<<"curlfactor:"<<mCurlFactor<<"\npagelength:"<<mPageLength<<"\nisheightmin:"<<mIsHeightMin<<
                  "\npagecutstartingpoint:"<<mPageCutStartingPoint;
    }
}

QSize DPageCurl::hostWidgetSize()
{
    return mSourceImg.size();
}

QRect DPageCurl::hostWidgetRect()
{
    return mSourceImg.rect();
}

QWidget* DPageCurl::hostWidget()
{
    return mSourceWidget;
}

QPixmap DPageCurl::hostWidgetAsPixmap()
{
    return QPixmap::fromImage(mSourceImg);
}

// if next page cut is not valid returns a null pixmap
QPixmap DPageCurl::nextPageCut()
{
    int curlLength = mPageCutCount*mCurlFactor;
    if(curlLength < mPageLength)
    {
      QRegion pageCutRegion = grabPageCut(mSourceImg,curlLength,curlLength);
      // Extract this region from widget to image
      QImage img(mSourceImg.size(),QImage::Format_ARGB32_Premultiplied);
      QPainter painter(&img);
      // Paint transparent background to image before rendering
      painter.fillRect(img.rect(),QBrush(QColor(Qt::transparent)));
//      painter.fillRect(img.rect(),QBrush(QColor(Qt::red)));
      mSourceWidget->render(&painter,QPoint(),pageCutRegion);
#ifdef QT_DEBUG
      img.save(KImagePageCut+QString().setNum(mPageCutCount),
               "png",KImageQualityHigh); // for debug
      qDebug()<<"Pagecut count:"<<mPageCutCount<<"\nCurl length:"<<curlLength;
#endif
       ++mPageCutCount;
       mCurrentPagecutImage = img;
      // return valid image
      return QPixmap::fromImage(img);
    }

// return null pixmap
return QPixmap();
}

QPixmap DPageCurl::nextPageCut__(QPointF pageCutPos)
{
    // This is pagecurl starting from top right
    int curlWidth = hostWidgetSize().width() - pageCutPos.x();

      QRegion pageCutRegion = grabPageCut(mSourceImg,curlWidth,curlWidth);
      // Extract this region from widget to image
      QImage img(mSourceImg.size(),QImage::Format_ARGB32_Premultiplied);
      QPainter painter(&img);
      // Paint transparent background to image before rendering
      painter.fillRect(img.rect(),QBrush(QColor(Qt::transparent)));
//      painter.fillRect(img.rect(),QBrush(QColor(Qt::red)));
      mSourceWidget->render(&painter,QPoint(),pageCutRegion);
#ifdef QT_DEBUG
      img.save(KImagePageCut+QString().setNum(mPageCutCount),
               "png",KImageQualityHigh); // for debug
      mPageCutCount++;
#endif
       mCurrentPagecutImage = img;
      // return valid image
      return QPixmap::fromImage(img);
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


QPixmap DPageCurl::nextCurlCut()
{
qDebug()<<__PRETTY_FUNCTION__;
    int curlCount = mPageCutCount - 1;
    int curlWidth = curlCount*mCurlFactor;

    // Copy required curl from page cut
    QImage img(mCurrentPagecutImage.copy(mCurrentPagecutImage.width()-curlWidth,
                                         0,
                                         curlWidth,curlWidth));
    QRect curlRect = img.rect();
    QPainter painter(&img);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.fillRect(curlRect,Qt::transparent);
    QVector<QPoint> curlPoints;
    curlPoints.append(curlRect.bottomLeft());
    curlPoints.append(curlRect.bottomRight());

    QPainterPath curlPath(curlRect.topLeft());
    for(int i=0;i<curlPoints.count();i++)
    {
        curlPath.lineTo(curlPoints.value(i));
    }
    curlPath.closeSubpath();

    QLinearGradient gradient(0,0,curlRect.width(),curlRect.height());
    gradient.setSpread(QGradient::ReflectSpread);
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.8, Qt::lightGray);
    QBrush curlBrush(gradient);
    painter.fillPath(curlPath,curlBrush);
    img.save("tempcurl_"+QString().setNum(nextCurlWidth())+".png","png",100); // debug
    return QPixmap::fromImage(img);
}


QPixmap DPageCurl::nextCurlCut__(QPointF curlPos)
{
qDebug()<<__PRETTY_FUNCTION__;

    // This is pagecurl starting from top right
    int curlWidth = hostWidgetSize().width() - curlPos.x();

    // Copy required curl from page cut
    QImage img(mCurrentPagecutImage.copy(mCurrentPagecutImage.width()-curlWidth,
                                         0,
                                         curlWidth,curlWidth));
    QRect curlRect = img.rect();
    QPainter painter(&img);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.fillRect(curlRect,Qt::transparent);
    QVector<QPoint> curlPoints;
    curlPoints.append(curlRect.bottomLeft());
    curlPoints.append(curlRect.bottomRight());

    QPainterPath curlPath(curlRect.topLeft());
    for(int i=0;i<curlPoints.count();i++)
    {
        curlPath.lineTo(curlPoints.value(i));
    }
    curlPath.closeSubpath();

    QLinearGradient gradient(0,0,curlRect.width(),curlRect.height());
    gradient.setSpread(QGradient::ReflectSpread);
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.8, Qt::lightGray);
    QBrush curlBrush(gradient);
    painter.fillPath(curlPath,curlBrush);

    // test
    static int count = 1;
    qDebug()<<"count:"<<count;
//    img.save("tempcurl_"+QString().setNum(count++)+".png","png",100); // debug
    // test

    return QPixmap::fromImage(img);
}

#define ADDCURVE
QPainterPath DPageCurl::nextCurlPath(QRectF curlRect)
{
    QPainterPath curlPath(curlRect.topLeft());
    curlPath.lineTo(curlRect.bottomRight());
#ifdef ADDCURVE
    const int KPercentOfX = 30;
    const int KPercentOfY = 10;
    QPointF currentPoint = curlRect.bottomRight();
    QPointF c1;
    c1.setX( currentPoint.x() - ((KPercentOfX*currentPoint.x())/100) );  // % of X
    c1.setY( currentPoint.y() - ((KPercentOfY*currentPoint.y())/100) );
    curlPath.cubicTo(c1,curlRect.bottomLeft(),curlRect.bottomLeft());
    currentPoint = curlRect.center();
    c1.setX( currentPoint.x() - ((KPercentOfX+60*currentPoint.x())/100) );  // % of X
    c1.setY( currentPoint.y() - ((KPercentOfY*currentPoint.y())/100) );
    curlPath.cubicTo(c1,curlRect.topLeft(),curlRect.topLeft());
curlPath.closeSubpath();
#else
  curlPath.lineTo(curlRect.bottomLeft());
  curlPath.closeSubpath();
#endif

    return curlPath;
}

int DPageCurl::nextCurlWidth()
{
    int curlCount = mPageCutCount - 1;
    return curlCount*mCurlFactor;
}

QImage DPageCurl::captureOriginal(QWidget* widget)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QImage img(widget->size(),QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);
    widget->render(&painter);
    return img;
}



QRegion DPageCurl::grabPageCut__(QImage image,int width,int height)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QPainter painter(&image);

    // Create page points through which we cut the page
    QVector<QPointF> pageCutPoints;

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
