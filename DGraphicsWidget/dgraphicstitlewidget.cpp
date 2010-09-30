#include <QDebug>
#include <QGraphicsLinearLayout>
#include "dgraphicstitlewidget.h"
#include "dgraphicspixmapwidget.h"
#include "dgraphicstextwidget.h"

DGraphicsTitleWidget::DGraphicsTitleWidget(QGraphicsItem *parent)
                     :QGraphicsWidget(parent)
{
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
    // do pixmap trimming here
    if(autoResize)
    {
    int trim = (20*rect().height()/100); //  % of height
    titlePixmap = titlePixmap.scaled(QSize(rect().height()-trim,rect().height()-trim),
                                     Qt::IgnoreAspectRatio,
                                     Qt::SmoothTransformation);
    }
    mTitlePixmap->pixmapItem()->setPixmap(titlePixmap);
}

void DGraphicsTitleWidget::addItemsToLayout()
{
    mLinearLayout->addItem(mTitlePixmap);
    mLinearLayout->addItem(mTitleText);
}
