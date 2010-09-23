#include <QDebug>
#include <QGraphicsWidget>
#include <QPainter>
#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsTextItem>
#include "dgraphicswidget.h"
#include "dgraphicstextitem.h"
#include "dgraphicspixmapitem.h"

DGraphicsWidget::DGraphicsWidget(QGraphicsItem* parent)
    : QGraphicsWidget(parent)
{
    setAttribute(Qt::WA_OpaquePaintEvent,true);
    setBackgroundColor(QColor(Qt::blue));
    mAnchorLayout = new QGraphicsAnchorLayout(this);
    mAnchorLayout->setSpacing(0);
    setLayout(mAnchorLayout);
    mTitleLayout = new QGraphicsLinearLayout(Qt::Horizontal);

    mCommandLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    mCommandLayout->setMaximumHeight(10);
    // add to anchor layout
    QGraphicsAnchor* anchor;
    anchor = mAnchorLayout->addAnchor(mTitleLayout,Qt::AnchorTop,
                                      mAnchorLayout,Qt::AnchorTop);

    anchor = mAnchorLayout->addAnchor(mCommandLayout,Qt::AnchorBottom,
                                      mAnchorLayout,Qt::AnchorBottom);

    mTextItem = new DGraphicsTextItem(QString(),this);
    mTextItem->textItem()->setPlainText("Hi!");
    mTitleLayout->addItem(mTextItem);
    addCommands();
}

DGraphicsWidget::~DGraphicsWidget()
{

}

void DGraphicsWidget::paint(QPainter *painter,
           const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    const int roundness(5);

    // Create a painterpath, this is the region in which we paint
    QPainterPath paintPath;
    paintPath.addRoundedRect(rect(),roundness,roundness);

    // Save the rect in which we paint since the size is static
    QRect paintRect = paintPath.boundingRect().toRect();
    painter->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    painter->fillPath(paintPath,QBrush(backgroundColor()));
}

void DGraphicsWidget::setTitleText(QString titleText)
{
    mTextItem->textItem()->setPlainText(titleText);
}

QString DGraphicsWidget::titleText()
{
    return mTextItem->textItem()->toPlainText();
}

void DGraphicsWidget::addContentLayout(QGraphicsLayoutItem* contentLayout)
{

    // content is between top and botton layouts
    QGraphicsAnchor* anchor = mAnchorLayout->addAnchor(contentLayout,Qt::AnchorTop,
                                                       mTitleLayout,Qt::AnchorBottom);
    anchor = mAnchorLayout->addAnchor(contentLayout,Qt::AnchorBottom,
                                      mCommandLayout,Qt::AnchorTop);
    mAnchorLayout->addAnchor(contentLayout,Qt::AnchorRight,
                             mAnchorLayout,Qt::AnchorRight);
    mAnchorLayout->addAnchor(contentLayout,Qt::AnchorLeft,
                             mAnchorLayout,Qt::AnchorLeft);


}

void DGraphicsWidget::addCommands()
{
    QPixmap pixmap(":/resource/images/close.png");
    qDebug()<<"pixmap is null "<<pixmap.isNull();
    DGraphicsPixmapItem* pixmapitem = new DGraphicsPixmapItem(pixmap,this);
    mCommandLayout->addItem(pixmapitem);
}

