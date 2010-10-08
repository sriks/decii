#ifndef DGRAPHICSLAYER_H
#define DGRAPHICSLAYER_H

#include <QGraphicsWidget>

class DPageCurl;
class DGraphicsLayer : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(QPointF curl READ curl WRITE setCurl)

public:
    DGraphicsLayer(DPageCurl* curl);
    void usePageCurl(bool use){mUsePageCurl = use;}
    bool isUsingPageCurl(){return mUsePageCurl;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPointF curl();
    void setCurl(QPointF curlPos);
private:
    DPageCurl* mPageCurl;
    bool mUsePageCurl;
};

#endif // DGRAPHICSLAYER_H
