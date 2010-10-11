#ifndef DPAGECURL_H
#define DPAGECURL_H

#include <QGraphicsWidget>


class DPageCurl : public QObject
{
    Q_OBJECT

public:
    DPageCurl(QObject *parent = 0);
    ~DPageCurl();

    void setWidget(QWidget* widget);
    QSize hostWidgetSize();
    QRect hostWidgetRect();
    QWidget* hostWidget();
    QPixmap hostWidgetAsPixmap();
    void doCurl(QWidget* widget,int width,int height);
    QImage captureOriginal(QWidget* widget);
    QPixmap nextPageCut();
    QPixmap nextPageCut__(QPointF curPos);
    QPixmap nextCurlCut();
    QPixmap nextCurlCut__(QPointF curlPos);
    QPainterPath nextCurlPath(QRectF curlRect);
    QRegion grabPageCut(QImage image,int width,int height);
    QRegion grabPageCut__(QImage image,int width,int height);
    int nextCurlWidth();
    void prepareNext(){mPageCutCount++;} // for test
private:
    bool mIsHeightMin;
    qreal mCurlFactor;
    int mPageLength;
    QImage mSourceImg;
    // start page cut from this point. QPainterPath startingpoint is set to this point
    QPointF mPageCutStartingPoint;
    // start curl from this point
    QPointF mPageCurlStartingPoint;
    int mPageCutCount;
    QWidget* mSourceWidget;
    QImage mCurrentPagecutImage;
};

#endif // DPAGECURL_H
