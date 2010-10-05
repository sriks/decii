#ifndef DPAGECURL_H
#define DPAGECURL_H

#include <QGraphicsWidget>


class DPageCurl : public QGraphicsWidget
{
    Q_OBJECT

public:
    DPageCurl(QGraphicsItem *parent = 0);
    ~DPageCurl();

    void doCurl(QWidget* widget,int width,int height);
    QImage captureOriginal(QWidget* widget);
    QRegion grabPageCut(QImage image,int width,int height);
};

#endif // DPAGECURL_H
