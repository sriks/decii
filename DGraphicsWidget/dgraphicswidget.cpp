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

const int KTitleHeightFactor = 25;
DGraphicsWidget::DGraphicsWidget(QGraphicsItem* parent)
    : QGraphicsWidget(parent)
{
    prepareWidget();
//    mTitleWidget = new DGraphicsTitleWidget(this);
//    prepareTitleWidget();
    mAnchorLayout = new QGraphicsAnchorLayout;
    mAnchorLayout->setSpacing(0);
    setLayout(mAnchorLayout);

    mCommandLayout = new QGraphicsLinearLayout(Qt::Horizontal);

    // add to anchor layout
    QGraphicsAnchor* anchor;
//    anchor = mAnchorLayout->addAnchor(mTitleWidget,Qt::AnchorTop,
//                                      mAnchorLayout,Qt::AnchorTop);
    anchor = mAnchorLayout->addAnchor(mCommandLayout,Qt::AnchorTop,
                                      mAnchorLayout,Qt::AnchorTop);
    anchor = mAnchorLayout->addAnchor(mCommandLayout,Qt::AnchorRight,
                                      mAnchorLayout,Qt::AnchorRight);
    addDefaultActions();
}

DGraphicsWidget::~DGraphicsWidget()
{
    qDebug()<<__PRETTY_FUNCTION__;
}

void DGraphicsWidget::paint(QPainter *painter,
           const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    static const int roundness(5);
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

void DGraphicsWidget::addContent(QGraphicsLayoutItem* content)
{
//    qreal height = ((100 - KTitleHeightFactor)*rect().height())/100;
//    content->setMaximumHeight(height);
//    // content is between top and botton layouts
//    QGraphicsAnchor* anchor;
//    anchor = mAnchorLayout->addAnchor(mTitleWidget,Qt::AnchorBottom,
//                             content,Qt::AnchorTop);
}

void DGraphicsWidget::addDefaultActions()
{
    DGraphicsPixmapWidget* closeCommandIcon = new DGraphicsPixmapWidget(QPixmap(":/resource/images/close.png"),this);
    connect(closeCommandIcon,SIGNAL(triggered()),this,SIGNAL(closeButtonClicked()));
    mCommandLayout->addItem(closeCommandIcon);
}

void DGraphicsWidget::prepareWidget()
{
    setAttribute(Qt::WA_OpaquePaintEvent,true);
    QPalette p = palette();
    p.setColor(QPalette::Base,Qt::blue);
    setPalette(p);
}

void DGraphicsWidget::prepareTitleWidget()
{
    qreal height = (KTitleHeightFactor*rect().height())/100;
    mTitleWidget->resize(rect().width(),height);
    QFont titleFont("Arial");
    titleFont.setBold(true);
    titleFont.setPixelSize(mTitleWidget->rect().height());
    setTitleFont(titleFont);
}
