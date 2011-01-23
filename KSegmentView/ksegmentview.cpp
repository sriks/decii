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
#include "ksegment.h"
#include "ksegmentwidget.h"
#include "ksegmentview.h"

/*!
    \class KSegmentView
    \brief A QGraphicsView derived class which encapsulates a container with a layout to hold segments.
           \ref KSegment, KSegmentWidget or any QGraphicsWidget can be added to this view.

    Internally this view has a container which is a \ref KSegment. Hence a \ref KSegment or \ref KSegmentWidget
    can be added directly into this view, which inturn arranges the items in the order in which they are added.

    \section usage Usage
    \code
        KSegment* labelSegment = new KSegment(Qt::Vertical);
        labelSegment->layout()->setSpacing(0);
        labelSegment->addItem(new QGraphicsTextItem("Title"));
        labelSegment->addItem(new QGraphicsTextItem("Subtitle"));

        KSegment* buttonSegment = new KSegment(Qt::Vertical);
        buttonSegment->layout()->setSpacing(10);
        buttonSegment->addItem(new QPushButton("Upload"));
        QPushButton* removeButton = new QPushButton("Remove");
        buttonSegment->addItem(removeButton);

        // Now add all segments to a segment widget.
        //
        KSegmentWidget* segmentWidget = new KSegmentWidget(Qt::Horizontal);
        // Segments apppear in the order in which they are added.
        segmentWidget->addItem(labelSegment);
        segmentWidget->addItem(buttonSegment);

        KSegmentView segmentView = new KSegmentView(Qt::Vertical);
        segmentView->addSegmentWidget(segmentWidget);
    \endcode

    \sa QGraphicsView QGraphicsWidget KSegment KSegmentWidget
**/

KSegmentView::KSegmentView(Qt::Orientation aOrientation,QWidget *aParent) :
    QGraphicsView(aParent)
{
    QGraphicsScene* s = new QGraphicsScene;
    setScene(s);
    mContainer = new KSegment(aOrientation);
    mContainer->setObjectName("container");
    mContainer->setPos(0,0);
    scene()->addItem(mContainer); // ownership is transferred to scene
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

/*!
  Adds a \ref KSegmentWidget to this view. Ownership is transferred.
  Since \ref KSegmentWidget has a visual appearance, once it is added the size policy is set
  so that it can grow horizontally, but fixed vertically. This is required for vertical layout
  since removing an item makes other items to grow vertically.

  \sa addSegment
**/
void KSegmentView::addSegmentWidget(KSegmentWidget* aSegmentWidget)
{
    aSegmentWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    aSegmentWidget->setParentContainer(mContainer->layout());
    mContainer->addItem(aSegmentWidget);
}

/*!
  Adds a \ref KSegment to this view. Ownership is transferred.
  Use this method to add a \ref KSegmentWidget with a defined sizepolicy.
  It is always a good practice to define the sizepolicy of items so that they are resized when other
  items in the layout are removed. By default, a layout tries to resize the items to fit the layout area.

  \sa addSegmentWidget
**/
void KSegmentView::addSegment(KSegment* aSegment)
{
    mContainer->addItem(aSegment);
}

void KSegmentView::resizeEvent(QResizeEvent *event)
{
    qDebug()<<__PRETTY_FUNCTION__;
}

// eof
