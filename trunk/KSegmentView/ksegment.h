/*
 *  Author: Srikanth Sombhatla
 *  Copyright 2010 Konylabs. All rights reserved.
 *
 */

#ifndef KSEGMENT_H
#define KSEGMENT_H

#include <QGraphicsWidget>
#include <QPointer>
class QGraphicsLinearLayout;
//class QPointer;
class KSegment : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit KSegment(Qt::Orientation aOrientation = Qt::Vertical,QGraphicsItem *aParent = 0);
    QGraphicsLinearLayout* layout() { return mLayout;}
    void addItem(QGraphicsWidget* aGraphicsLayoutItem);
    void addItem(QGraphicsItem* aGraphicsItem);
    void addItem(QWidget* aWidget);
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

protected slots:
    void onHeightChanged();

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    float contentHieght() const {return mContentHeight;}

private:
   QList< QPointer<QGraphicsWidget> > mContentList;
   QGraphicsLinearLayout* mLayout;
   float mContentHeight;
};

#endif // KSEGMENT_H
