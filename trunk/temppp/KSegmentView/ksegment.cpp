/*
 *  Author: Srikanth Sombhatla
 *  Copyright 2010 Konylabs. All rights reserved.
 *
 */

#include <QGraphicsSceneResizeEvent>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QDesktopWidget>
#include <QDebug>
#include "ksegment.h"

/*!
  \class KSegment
  \brief Atomic segment with a layout into which any widget can be added.
         This segment don't have a visual appearance, but just acts as a container to hold widgets
         within a layout. For segment with visual appearance refer to KSegmentWidget.

         To access the implict layout in this segment use layout().

  \section usage Usage
  \code
        KSegment* mySegment = new KSegment(Qt::Vertical);
        mySegment->layout()->setSpacing(0);
        mySegment->addItem(new QGraphicsTextItem("Title"));
        mySegment->addItem(new QGraphicsTextItem("Subtitle"));
        // QWidgets can also be directly added.
        mySegment->addItem(new QPushButton("Upload"));
  \endcode

  \sa KSegmentWidget
**/

KSegment::KSegment(Qt::Orientation aOrientation,QGraphicsItem *aParent) :
    QGraphicsWidget(aParent)
{
    mLayout = new QGraphicsLinearLayout(aOrientation,this);
    setLayout(mLayout);
}

void KSegment::addItem(QGraphicsWidget* aItem)
{
    mLayout->addItem(aItem);
}

void KSegment::addItem(QGraphicsItem* aGraphicsItem)
{
    QGraphicsWidget* parent = new QGraphicsWidget(this);
    aGraphicsItem->setParentItem(parent);
    mLayout->addItem(parent);
}

void KSegment::addItem(QWidget* aWidget)
{
    QGraphicsProxyWidget* proxyWidget = new QGraphicsProxyWidget(this);
    // Proxywidget shares ownership. Hence if proxy is destroyed, it destroys the embedded widget also.
    proxyWidget->setWidget(aWidget);
    mLayout->addItem(proxyWidget);
}

// eof
