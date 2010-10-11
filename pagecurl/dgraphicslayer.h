#ifndef DGRAPHICSLAYER_H
#define DGRAPHICSLAYER_H

#include <QGraphicsWidget>

class DPageCurl;
class DGraphicsLayer : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(QPointF curl READ curl WRITE setCurl)
    Q_PROPERTY(QPointF pageCut READ pageCut WRITE setPageCut)

public:
    DGraphicsLayer(DPageCurl* curl,bool isCurl);
    void usePageCurl(bool use){mUsePageCurl = use;}
    bool isUsingPageCurl(){return mUsePageCurl;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPointF curl();
    void setCurl(QPointF curlPos);
    QPointF pageCut();
    void setPageCut(QPointF pageCutPos);

private:
    DPageCurl* mPageCurl;
    bool mUsePageCurl;
    QPointF mPageCutPos;
};

#endif // DGRAPHICSLAYER_H
