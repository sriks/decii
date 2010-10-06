#ifndef DPAGECURL_H
#define DPAGECURL_H

#include <QGraphicsWidget>


class DPageCurl : public QGraphicsWidget
{
    Q_OBJECT

public:
    DPageCurl(QGraphicsItem *parent = 0);
    ~DPageCurl();

    void setWidget(QWidget* widget);
    void doCurl(QWidget* widget,int width,int height);
    QImage captureOriginal(QWidget* widget);
    QPixmap nextPageCut();
    QPixmap nextCurlCut();
    QRegion grabPageCut(QImage image,int width,int height);

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
};

#endif // DPAGECURL_H
