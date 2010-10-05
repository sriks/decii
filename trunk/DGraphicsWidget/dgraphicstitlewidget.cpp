#include <QDebug>
#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QGraphicsSceneResizeEvent>
#include "dgraphicstitlewidget.h"
#include "dgraphicspixmapwidget.h"
#include "dgraphicstextwidget.h"

const int KTitlePixmapWidthPercentage = 40;
const int KTitleTextWidthPercentage = 100-KTitlePixmapWidthPercentage;

DGraphicsTitleWidget::DGraphicsTitleWidget(QGraphicsItem *parent)
                     :QGraphicsWidget(parent)
{
    mTitlePixmapAutoResize = false;
    mTitleText = new DGraphicsTextWidget("",this);
    mTitlePixmap = new DGraphicsPixmapWidget(QPixmap(),this);
    mLinearLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    setLayout(mLinearLayout);
    setOwnedByLayout(false);
    addItemsToLayout();
}

DGraphicsTitleWidget::~DGraphicsTitleWidget()
{
    qDebug()<<__PRETTY_FUNCTION__;
}

void DGraphicsTitleWidget::setTitleText(QString titleText)
{
    mTitleText->textItem()->setPlainText(titleText);
}

void DGraphicsTitleWidget::setTitlePixmap(QPixmap titlePixmap,bool autoResize)
{
    qDebug()<<__PRETTY_FUNCTION__;
    mTitlePixmapAutoResize = autoResize;
    if(mTitlePixmapAutoResize)
    {
    titlePixmap = resizePixmap(titlePixmap);
    }
    mTitlePixmap->pixmapItem()->setPixmap(titlePixmap);
}

QPixmap DGraphicsTitleWidget::resizePixmap(QPixmap pixmap)
{
    qDebug()<<__PRETTY_FUNCTION__;
    qDebug()<<"autoresize to "<<rect().size();
    int factor = qMin(rect().width(),rect().height());
    qDebug()<<"factor:"<<factor;
    int trim = (20*factor/100); //  % of height
    factor -= trim;
    return pixmap.scaled(factor/2,factor/2,
                               Qt::IgnoreAspectRatio,
                               Qt::SmoothTransformation);
}

void DGraphicsTitleWidget::addItemsToLayout()
{
    mLinearLayout->setSpacing(0);
    mLinearLayout->addItem(mTitlePixmap);
    mLinearLayout->setStretchFactor(mTitlePixmap,0);
    mLinearLayout->addItem(mTitleText);
}

void DGraphicsTitleWidget::paint(QPainter *painter,
                                const QStyleOptionGraphicsItem *option,
                                QWidget *widget)
{
#ifdef DEBUG_PAINT
    painter->setPen(QPen(QColor(Qt::green)));
    painter->drawRect(rect());
#endif
}

void DGraphicsTitleWidget::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    qDebug()<<__PRETTY_FUNCTION__;
    qDebug()<<"newsize:"<<event->newSize();
    setTitlePixmap(mTitlePixmap->pixmapItem()->pixmap(),mTitlePixmapAutoResize);
    mLinearLayout->invalidate();
}

// eof
