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

// TODO: remove unnecessary debug statements
/*
  This is an internal container class to hold QGraphicsItems so that they are sizehint aware.
  Apart from identifying as internal container, all other methods are inherited from base class.
  */
class KSegmentInteralContainer : public KSegment
{
public:
    KSegmentInteralContainer(Qt::Orientation aOrientation = Qt::Vertical,QGraphicsItem *aParent = 0)
                   :KSegment(aOrientation,aParent)
    {
            this->setInternalContainer(true);
            this->setObjectName("____internalcontainer");
    }
};

//#define DEBUG_DRAW
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
    mContentHeight(0),
    mInternalContainer(false)
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
    KSegmentInteralContainer* parent = new KSegmentInteralContainer(Qt::Vertical,this);
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

void KSegment::onHeightChanged() const
{
    qDebug()<<__PRETTY_FUNCTION__;
    // TODO: add support for width change also.

    if(internalContainer())
    {
        mContentHeight = 0;
        qDebug()<<objectName();
        qDebug()<<"childitems:"<<this->childItems();
        for(int i=0;i<this->childItems().count();++i)
        {
            qDebug()<<"item height:"<<childItems().at(i)->boundingRect().height();
            mContentHeight += childItems().at(i)->boundingRect().height();
        }
        qDebug()<<"calculated contentheight:"<<mContentHeight;
    }
    else
    {
        qDebug()<<"items in layout:"<<mLayout->count();
        mContentHeight = 0;
        for(int i=0;i<mLayout->count();++i)
        {
            qDebug()<<mLayout->itemAt(i)->graphicsItem()->boundingRect().height();
            mContentHeight += mLayout->itemAt(i)->graphicsItem()->boundingRect().height();
        }
        qDebug()<<"calculated contentheight:"<<mContentHeight;
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

QSizeF KSegment::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    qDebug()<<__PRETTY_FUNCTION__;
    // Ideally heightchanged should be emitted after adding all items to container, but not happening.
    // Hence explicitly calling heightchanged here.
    onHeightChanged();
    return QSizeF(QDesktopWidget().size().width(),mContentHeight);
}

// eof
