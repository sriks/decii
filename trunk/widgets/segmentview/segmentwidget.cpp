/*
 *  Author: Srikanth Sombhatla
 *  Copyright 2010 Konylabs. All rights reserved.
 *
 */

#include <QPainter>
#include <QPainterPath>
#include <QGraphicsPathItem>
#include <QGraphicsSceneResizeEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsLinearLayout>
#include <QDebug>
#include "segmentwidget.h"

// Make sure no item will have a z-order greater than this
const int KSelectionZOrder = 10;

// Make sure no item will have a z-order less than this
const int KBackgroundZOrder = -10;

/*!
  \class SegmentWidget
  \brief This class extends \ref Segment. Apart from all the properties of a Segment, this class
  additionally have a visual appearance and selection.

  \section usage Usage
  \code
        Segment* labelSegment = new Segment(Qt::Vertical);
        labelSegment->layout()->setSpacing(0);
        labelSegment->addItem(new QGraphicsTextItem("Title"));
        labelSegment->addItem(new QGraphicsTextItem("Subtitle"));

        Segment* buttonSegment = new Segment(Qt::Vertical);
        buttonSegment->layout()->setSpacing(10);
        buttonSegment->addItem(new QPushButton("Upload"));
        QPushButton* removeButton = new QPushButton("Remove");
        buttonSegment->addItem(removeButton);

        // Now add all segments to a segment widget.
        //
        SegmentWidget* segmentWidget = new SegmentWidget(Qt::Horizontal);
        // Segments apppear in the order in which they are added.
        segmentWidget->addItem(labelSegment);
        segmentWidget->addItem(buttonSegment);
  \endcode

  \sa Segment SegmentView
**/

SegmentWidget::SegmentWidget(Qt::Orientation aOrientation,
                               QGraphicsItem *aParent)
               :Segment(aOrientation,aParent),
                mRoundness(10),
                mSelectOnClicked(true),
                mTapPressPos(-1,-1)
{
    mSelection = new QGraphicsPathItem(this);
    mSelection->setVisible(false);
    mSelection->setBrush(QBrush(Qt::NoBrush)); // ensure there is no brush
    QPen selectionPen(QColor("#99FFAA"));
    selectionPen.setWidthF(2.5);
    selectionPen.setJoinStyle(Qt::RoundJoin);
    selectionPen.setJoinStyle(Qt::RoundJoin);
    mSelection->setPen(selectionPen);
    mSelection->setZValue(KSelectionZOrder);
}

void SegmentWidget::select()
{
    mSelection->setVisible(true);
}

void SegmentWidget::deselect()
{
    mSelection->setVisible(false);
}

void SegmentWidget::toggleSelection()
{
    if(isSelectOnClicked())
    {
        if(isSelected()){deselect();}
        else{select();}
    }
}

/*!
    Removes the item with an animation effect of scaling and opacity.
    Item is deleted.
**/
void SegmentWidget::removeWithAnimation()
{
    int animDuration = 1000/2;
    QPropertyAnimation* propAnim = new QPropertyAnimation(this,"scale",this);
    propAnim->setStartValue(scale());
    propAnim->setEndValue(0);
    propAnim->setDuration(animDuration);
    propAnim->setEasingCurve(QEasingCurve::InCubic);

    QPropertyAnimation* propAnim2 = new QPropertyAnimation(this,"opacity",this);
    propAnim2->setStartValue(opacity());
    propAnim2->setEndValue(0);
    propAnim2->setDuration(animDuration);

    QParallelAnimationGroup* animGroup = new QParallelAnimationGroup(this);
    animGroup->addAnimation(propAnim);
    animGroup->addAnimation(propAnim2);
    connect(animGroup,SIGNAL(finished()),this,SLOT(remove()));
    animGroup->start(QPropertyAnimation::DeleteWhenStopped);
}

/*!
  Removes the item from parent container and it is deleted.
  When a \ref SegmentWidget is added to a \ref SegmentView, the view will set its layout as a
  parent container through SegmentWidget::setParentContainer(). When removing, \ref SegmentWidget removes itself
  from the parent container before closing itself. This is required so that when an item is removed, the
  layout adjusts itself to fill the gap.
  **/
void SegmentWidget::remove()
{
    qDebug()<<__PRETTY_FUNCTION__;
    if(mContainerLayout)
    {
        mContainerLayout->removeItem(this);
    }
    this->close();
}

void SegmentWidget::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event);
    QPainterPath painterPath;
    painterPath.addRoundRect(rect(),mRoundness);
    mSelection->setPath(painterPath);
}

void SegmentWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    mTapPressPos = event->scenePos();
}

void SegmentWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // If we want to enable highlight on pressevent, but selection only upon
    // releaseevent provided that press and release are in the same item within acceptable deviation,
    // then calculate the delta between press,release and apply selection.
    emit clicked(event->scenePos());
    toggleSelection();
}

void SegmentWidget::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // TODO: Option 1 : Since background don't change do offsrceen drawing and just paint the pixmap here.
    //       Option 2 : Background and selection border can be graphic items (QGraphicsRectItem) created with
    //                  lower z order so that they appear as background.

    // The following is a quick and dirty approach.
    painter->setRenderHint(QPainter::Antialiasing,true);
    QLinearGradient gradient(0, 0, 0, size().height());
    gradient.setSpread(QGradient::ReflectSpread);
    gradient.setColorAt(0.0, QColor("#444444"));
    gradient.setColorAt(0.7, QColor("#888888"));
    QBrush brush(gradient);
    painter->setBrush(brush);
    QPainterPath p;
    if(mRoundness)
    {
        p.addRoundedRect(rect(),mRoundness,mRoundness);
        painter->setClipPath(p);
    }
    else
    {
        p.addRect(rect());
    }
    painter->fillPath(p,brush);
}
// eof
