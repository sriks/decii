/*
 *  Author: Srikanth Sombhatla
 *  Copyright 2010 Konylabs. All rights reserved.
 *
 */

#ifndef SegmentVIEW_H
#define SegmentVIEW_H

#include <QGraphicsView>

class Segment;
class SegmentWidget;
class QGraphicsWidget;
class QGraphicsSceneMouseEvent;
class QGestureEvent;
class SegmentView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SegmentView(Qt::Orientation aOrientation = Qt::Vertical,QWidget *aParent = 0);
    void addSegmentWidget(SegmentWidget* aSegmentWidget);
    void addSegment(Segment* aSegment);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Segment* mContainer;
    QPointF mTapPressPos;
};

#endif
