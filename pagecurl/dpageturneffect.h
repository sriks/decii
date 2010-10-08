#ifndef DPAGETURNEFFECT_H
#define DPAGETURNEFFECT_H

#include <QGraphicsView>

class QGraphicsPixmapItem;
class DPageCurl;
class DGraphicsLayer;
class DPageTurnEffect : public QGraphicsView
{
public:
    DPageTurnEffect(QObject* parent=0);
    void setWidget(QWidget* hostWidget);
    void startEffect();

    void testAnimation();
protected:
    void timerEvent(QTimerEvent *event);
private:
    void makeTransparent();
private:
    DPageCurl* mPageCurl;
    QGraphicsPixmapItem* mPagePixmap;
    QGraphicsPixmapItem* mCurlPixmap;
    DGraphicsLayer* mCurlLayer;
    int mTimerId;
};

#endif // DPAGETURNEFFECT_H
