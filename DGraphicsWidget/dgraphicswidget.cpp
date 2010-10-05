#include <QDebug>
#include <QGraphicsWidget>
#include <QPainter>
#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsTextItem>
#include <QApplication>
#include "dgraphicswidget.h"
#include "dgraphicstextwidget.h"
#include "dgraphicspixmapwidget.h"
#include "dgraphicstitlewidget.h"

const int KTitleHeightPercentage = 25;
const int KContentHeightPercenrage = 100 - KTitleHeightPercentage;

DGraphicsWidget::DGraphicsWidget(QGraphicsItem* parent)
    : QGraphicsWidget(parent)
{
    mContentWidget = NULL;
    mTitleWidget = NULL;
    mTitleWidget = new DGraphicsTitleWidget(this);
    mTitleLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    mCommandLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    mAnchorLayout = new QGraphicsAnchorLayout;
    addDefaultActions();
    prepareWidget();
    prepareTitleWidget();
    addItemsToLayout();
}

void DGraphicsWidget::addItemsToLayout()
{
    mTitleLayout->setSpacing(0);
    mAnchorLayout->setSpacing(0);
    mTitleLayout->addItem(mTitleWidget);
    mTitleLayout->addItem(mCommandLayout);

    QGraphicsAnchor* anchor;
    anchor = mAnchorLayout->addAnchor(mTitleLayout,Qt::AnchorTop,
                                      mAnchorLayout,Qt::AnchorTop);
////    anchor = mAnchorLayout->addAnchor(mCommandLayout,Qt::AnchorTop,
////                                      mAnchorLayout,Qt::AnchorTop);
//    anchor = mAnchorLayout->addAnchor(mCommandLayout,Qt::AnchorRight,
//                                      mAnchorLayout,Qt::AnchorRight);
    setLayout(mAnchorLayout);

}

DGraphicsWidget::~DGraphicsWidget()
{
    qDebug()<<__PRETTY_FUNCTION__;
}

void DGraphicsWidget::paint(QPainter *painter,
           const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    static const int roundness(10);
    // Create a painterpath, this is the region in which we paint
    QPainterPath paintPath;
    paintPath.addRoundedRect(rect(),roundness,roundness);
    painter->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    QColor baseColor = palette().color(QPalette::Base);
    QLinearGradient linearGradient(QPointF(rect().width()/2,0),
                                   QPointF(rect().width()/2,rect().height()));
    linearGradient.setSpread(QGradient::ReflectSpread);
    linearGradient.setColorAt(0,baseColor.lighter(165));
    linearGradient.setColorAt(0.6,baseColor.lighter(110));
    linearGradient.setColorAt(1,baseColor);
    painter->fillPath(paintPath,QBrush(linearGradient));
}

void DGraphicsWidget::setTitleText(QString titleText)
{
    mTitleWidget->setTitleText(titleText);
}

void DGraphicsWidget::setTitleFont(QFont font)
{
    mTitleWidget->textWidget()->textItem()->setFont(font);
}

void DGraphicsWidget::setTitlePixmap(QPixmap pixmap,bool autoResize)
{
    mTitleWidget->setTitlePixmap(pixmap,autoResize);
}

void DGraphicsWidget::addContent(QGraphicsWidget* contentWidget)
{
    prepareContentWidget(contentWidget);
    QGraphicsAnchor* anchor;
    anchor = mAnchorLayout->addAnchor(mTitleLayout,Qt::AnchorBottom,
                                      contentWidget,Qt::AnchorTop);
    mContentWidget = contentWidget;
}


void DGraphicsWidget::addDefaultActions()
{
    DGraphicsPixmapWidget* closeCommandIcon =
            new DGraphicsPixmapWidget(QPixmap(":/resource/images/close.png"));
    mCommandLayout->addItem(closeCommandIcon);
    connect(closeCommandIcon,SIGNAL(triggered()),this,SIGNAL(closeButtonClicked()));

    connect(closeCommandIcon,SIGNAL(triggered()),this,SLOT(testResize()));

}

void DGraphicsWidget::prepareWidget()
{
    resize(400,400);
    setAttribute(Qt::WA_OpaquePaintEvent,true);
    QPalette p = palette();
    p.setColor(QPalette::Base,Qt::blue);
    setPalette(p);
}

void DGraphicsWidget::prepareTitleWidget()
{
    qreal height = (KTitleHeightPercentage*rect().height())/100;
    mTitleWidget->setPreferredSize(QSizeF(size().width(),height));
    QFont font("arial");
    font.setPixelSize(15);
    mTitleWidget->textWidget()->textItem()->setFont(font);
}

void DGraphicsWidget::prepareContentWidget(QGraphicsWidget* content)
{
    qDebug()<<__PRETTY_FUNCTION__;
    // resize content
    if(content)
    {
    QSizeF newsize(size());
    qreal height = (KTitleHeightPercentage*newsize.height())/100;
    content->setPreferredSize(newsize.width(),height);
    qDebug()<<"content resized";
    }
}

void DGraphicsWidget::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    qDebug()<<__PRETTY_FUNCTION__;
//    mAnchorLayout->invalidate();
    prepareTitleWidget();
    prepareContentWidget(mContentWidget);
}

// eof
