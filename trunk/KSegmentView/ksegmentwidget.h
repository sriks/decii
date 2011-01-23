/*
 *  Author: Srikanth Sombhatla
 *  Copyright 2010 Konylabs. All rights reserved.
 *
 */

#ifndef KSEGMENTWIDGET_H
#define KSEGMENTWIDGET_H
#include <QGraphicsWidget>
#include "ksegment.h"

class QGraphicsItem;
class QGraphicsPathItem;
class QGraphicsPixmapItem;
class KSegmentWidget : public KSegment
{
    Q_OBJECT
public:
    explicit KSegmentWidget(Qt::Orientation aOrientation = Qt::Vertical,QGraphicsItem *aParent = 0);
    bool isSegmentWidget(){return true;}
    void setRoundness(int aRoundness){mRoundness = aRoundness;}
    int roundness(){return mRoundness;}
    void select();
    void deselect();
    void toggleSelection();
    bool isSelected() const{return mSelection->isVisible();}
    void setSelectOnClicked(bool aVal){mSelectOnClicked = aVal;}
    bool isSelectOnClicked(){return mSelectOnClicked;}
    void setParentContainer(QGraphicsLinearLayout* aContainerLayout){mContainerLayout = aContainerLayout;}
    QGraphicsLinearLayout* parentContainer(){return mContainerLayout;}
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

public slots:
    void removeWithAnimation();
    void remove();

signals:
    void clicked(int index, QPointF clickPos);

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QGraphicsLinearLayout*      mContainerLayout;
    QGraphicsPathItem*          mSelection;
    QGradient                   mBackgroundGradient;
    QPointF                     mTapPressPos;
    int                         mRoundness;
    bool                        mSelectOnClicked;
};

#endif // KSEGMENTWIDGET_H
