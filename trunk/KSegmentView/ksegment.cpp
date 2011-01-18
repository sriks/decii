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
#include <QPainter>
#include <QList>
#include "ksegment.h"

#define DEBUG_DRAW
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
    QGraphicsWidget(aParent),
    mContentHeight(0)
{
    mLayout = new QGraphicsLinearLayout(aOrientation,this);
    setLayout(mLayout);
}

void KSegment::addItem(QGraphicsWidget* aItem)
{
    mLayout->addItem(aItem);
    mContentList.append(aItem);
    connect(aItem,SIGNAL(heightChanged()),this,SLOT(onHeightChanged()));
}

void KSegment::addItem(QGraphicsItem* aGraphicsItem)
{
    QGraphicsWidget* parent = new QGraphicsWidget(this);
    aGraphicsItem->setParentItem(parent);
    addItem(parent);
}

void KSegment::addItem(QWidget* aWidget)
{
    QGraphicsProxyWidget* proxyWidget = new QGraphicsProxyWidget(this);
    // Proxywidget shares ownership. Hence if proxy is destroyed, it destroys the embedded widget also.
    proxyWidget->setWidget(aWidget);
    addItem(proxyWidget);
}

void KSegment::onHeightChanged()
{
    qDebug()<<__PRETTY_FUNCTION__;
    if(Qt::Vertical == mLayout->orientation())
    {
        mContentHeight = 0;
        for(int i=0;i<mLayout->count();++i)
        {
            mContentHeight += mLayout->itemAt(i)->graphicsItem()->boundingRect().height();
            qDebug()<<mLayout->itemAt(i)->graphicsItem()->boundingRect().height();
        }
//        qDebug()<<"contentheight:"<<mContentHeight;
//        qDebug()<<"layoutcontent:"<<mLayout->contentsRect().height();

//        qDebug()<<"calculating with widget list : "<<mContentList.count();
//        mContentHeight = 0;
//        for(int i=0;i<mContentList.count();++i)
//        {
//            qDebug()<<mContentList.at(i)->size().height();
//            mContentHeight += mContentList.at(i)->size().height();
//            qDebug()<<"contentheight:"<<mContentHeight;
//        }
    }
}

void KSegment::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    qDebug()<<__PRETTY_FUNCTION__;
    qDebug()<<"size:"<<size();
}


void KSegment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
#ifdef DEBUG_DRAW
//    qDebug()<<__PRETTY_FUNCTION__;
    if("container" == objectName())
    {
        painter->setBrush(QBrush(QColor(Qt::blue)));
        painter->drawRect(rect());
    }
#endif
}

// eof
