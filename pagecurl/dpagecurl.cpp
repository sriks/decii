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
const QString KImageCurl("curl.jpg");
const QString KImagePage("page.jpg");
const QString KImageCurlTmp("curltmp");
const QString KImageDotFormat(".jpg");

DPageCurl::DPageCurl(QGraphicsItem *parent)
    : QGraphicsWidget(parent)
{

}

DPageCurl::~DPageCurl()
{

}

void DPageCurl::doCurl(QWidget* widget,int width,int height)
{
    qDebug()<<__PRETTY_FUNCTION__;
    widget->setVisible(false);
    QImage sourceimg = captureOriginal(widget);
    qDebug()<<"Source Image Saved:"<<sourceimg.save(KImageSource);


    QRegion pageCutRegion = grabPageCut(sourceimg,width,height);
    QImage img(widget->size(),QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);
    widget->render(&painter,QPoint(),pageCutRegion);
    img.save("pagecut.jpg");
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
    QImage tmpImage(image);
    QPainter painter(&tmpImage);
    QPointF pointTopRight = image.rect().topRight();

    // Create page points through which we cut the page
    QVector<QPointF> pageCutPoints;
    pageCutPoints.append(QPointF(pointTopRight.x()-width,pointTopRight.y())); // point A
    pageCutPoints.append(QPointF(pointTopRight.x(),pointTopRight.y()+height)); // point B
    pageCutPoints.append(image.rect().bottomRight());
    pageCutPoints.append(image.rect().bottomLeft());

    // add to path
    QPointF startingPoint = image.rect().topLeft();
    QPainterPath pageCutPath(startingPoint); // start path from top left
    for(int i=0;i<pageCutPoints.count();i++)
    {
        pageCutPath.lineTo(pageCutPoints.value(i));
    }
    // close the path with starting point
    pageCutPath.closeSubpath();

    painter.fillPath(pageCutPath,QBrush(QColor(Qt::red)));
    qDebug()<<"tmpsave:"<<tmpImage.save(KImageCurlTmp+"1"+KImageDotFormat);
    painter.setClipPath(pageCutPath,Qt::ReplaceClip);
    return painter.clipRegion();

//return pageCutPath;
//    tmpImage.mirrored(true,false).save("mirror_h.jpg");
//    tmpImage.mirrored(false,true).save("mirror_v.jpg");
}

//eof
