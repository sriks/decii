/*
 *  Author: Srikanth Sombhatla
 *  Copyright 2010 Konylabs. All rights reserved.
 *
 */

#ifndef Segment_H
#define Segment_H

#include <QGraphicsWidget>
class QGraphicsLinearLayout;
class Segment : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Segment(Qt::Orientation aOrientation = Qt::Vertical,QGraphicsItem *aParent = 0);
    QGraphicsLinearLayout* layout() { return mLayout;}
    void addItem(QGraphicsWidget* aGraphicsLayoutItem);
    void addItem(QGraphicsItem* aGraphicsItem);
    void addItem(QWidget* aWidget);

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);
private:
   QGraphicsLinearLayout* mLayout;
};

#endif // Segment_H
