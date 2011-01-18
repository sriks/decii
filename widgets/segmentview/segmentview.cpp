/*
 *  Author: Srikanth Sombhatla
 *  Copyright 2010 Konylabs. All rights reserved.
 *
 */

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QGestureEvent>
#include <QPanGesture>
#include <QDebug>
#include "segment.h"
#include "segmentwidget.h"
#include "segmentview.h"

/*!
    \class SegmentView
    \brief A QGraphicsView derived class which encapsulates a container with a layout to hold segments.
           \ref Segment, SegmentWidget or any QGraphicsWidget can be added to this view.

    Internally this view has a container which is a \ref Segment. Hence a \ref Segment or \ref SegmentWidget
    can be added directly into this view, which inturn arranges the items in the order in which they are added.

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

        SegmentView segmentView = new SegmentView(Qt::Vertical);
        segmentView->addSegmentWidget(segmentWidget);
    \endcode

    \sa QGraphicsView QGraphicsWidget Segment SegmentWidget
**/

SegmentView::SegmentView(Qt::Orientation aOrientation,QWidget *aParent) :
    QGraphicsView(aParent)
{
    QGraphicsScene* s = new QGraphicsScene;
    setScene(s);
    mContainer = new Segment(aOrientation);
    scene()->addItem(mContainer); // ownership is transferred to scene
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

/*!
  Adds a \ref SegmentWidget to this view. Ownership is transferred.
  Since \ref SegmentWidget has a visual appearance, once it is added the size policy is set
  so that it can grow horizontally, but fixed vertically. This is required for vertical layout
  since removing an item makes other items to grow vertically.

  \sa addSegment
**/
void SegmentView::addSegmentWidget(SegmentWidget* aSegmentWidget)
{
    aSegmentWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    aSegmentWidget->setParentContainer(mContainer->layout());
    mContainer->addItem(aSegmentWidget);
}

/*!
  Adds a \ref Segment to this view. Ownership is transferred.
  Use this method to add a \ref SegmentWidget with a defined sizepolicy.
  It is always a good practice to define the sizepolicy of items so that they are resized when other
  items in the layout are removed. By default, a layout tries to resize the items to fit the layout area.

  \sa addSegmentWidget
**/
void SegmentView::addSegment(Segment* aSegment)
{
    mContainer->addItem(aSegment);
}

void SegmentView::resizeEvent(QResizeEvent *event)
{
    qDebug()<<__PRETTY_FUNCTION__;
    qDebug()<<"newsize:"<<event->size();
    if(mContainer->size() != event->size())
    {
    mContainer->resize(event->size().width(),mContainer->size().height());
    }
}

// eof
