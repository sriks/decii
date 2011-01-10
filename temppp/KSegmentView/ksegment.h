/*
 *  Author: Srikanth Sombhatla
 *  Copyright 2010 Konylabs. All rights reserved.
 *
 */

#ifndef KSEGMENT_H
#define KSEGMENT_H

#include <QGraphicsWidget>
class QGraphicsLinearLayout;
class KSegment : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit KSegment(Qt::Orientation aOrientation = Qt::Vertical,QGraphicsItem *aParent = 0);
    QGraphicsLinearLayout* layout() { return mLayout;}
    void addItem(QGraphicsWidget* aGraphicsLayoutItem);
    void addItem(QGraphicsItem* aGraphicsItem);
    void addItem(QWidget* aWidget);

private:
   QGraphicsLinearLayout* mLayout;
};

#endif // KSEGMENT_H
