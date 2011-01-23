/*
 *  Author: Srikanth Sombhatla
 *  Copyright 2010 Konylabs. All rights reserved.
 *
 */

#ifndef KSEGMENTVIEW_H
#define KSEGMENTVIEW_H

#include <QGraphicsView>

class KSegment;
class KSegmentWidget;
class QGraphicsWidget;
class QGraphicsSceneMouseEvent;
class QGestureEvent;
class KSegmentView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit KSegmentView(Qt::Orientation aOrientation = Qt::Vertical,QWidget *aParent = 0);
    void addSegmentWidget(KSegmentWidget* aSegmentWidget);
    void addSegment(KSegment* aSegment);
    KSegment* container(){return mContainer;}

protected:
    void resizeEvent(QResizeEvent *event);

private:
    KSegment* mContainer;
    QPointF mTapPressPos;
};

#endif // KSEGMENTEDUI_H
