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

const int KTitleHeightPercentage = 10;
const int KContentHeightPercenrage = 100 - KTitleHeightPercentage;

DGraphicsWidget::DGraphicsWidget(QGraphicsItem* parent)
    : QGraphicsWidget(parent),
      mGlassEffect(false)
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

    if(glassEffect())
    {
    // Save painter state as we are about to change it
    painter->save();

    // Create base gradient
    QLinearGradient gradient(0, 0, 0, rect().height());
    gradient.setSpread(QGradient::ReflectSpread);
    gradient.setColorAt(0.0, QColor(Qt::gray));
    gradient.setColorAt(0.6, QColor(Qt::black));
    QBrush brush(gradient);
    painter->setBrush(brush);
    painter->fillPath(paintPath,brush);

    // glass highligh
    painter->setBrush(QBrush(Qt::gray));
    painter->setPen(QPen(QBrush(Qt::white), 0.1));
    painter->setOpacity(0.30);

//    int startAngle = 30 * 16;
//    int spanAngle = 120 * 16;
    painter->drawRect(1,1,rect().width()-2,rect().height()/2);
    // restore painter to last saved state
    painter->restore();
    }

    else
    {
        QLinearGradient linearGradient(QPointF(rect().width()/2,0),
                                       QPointF(rect().width()/2,rect().height()));
        linearGradient.setSpread(QGradient::ReflectSpread);
        linearGradient.setColorAt(0,baseColor.lighter(165));
        linearGradient.setColorAt(0.6,baseColor.lighter(110));
        linearGradient.setColorAt(1,baseColor);
        painter->fillPath(paintPath,QBrush(linearGradient));
        painter->drawPath(paintPath);
    }
}

void DGraphicsWidget::setTitleText(QString titleText)
{
    mTitleWidget->setTitleText(titleText);
}

void DGraphicsWidget::setTitleFont(QFont font)
{
//    mTitleWidget->textWidget()->textItem()->setFont(font);
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

QSizeF DGraphicsWidget::contentWidgetSize()
{
    QSizeF newsize(size());
    qreal newheight = (KContentHeightPercenrage*size().height())/100;
    newheight -= (newheight*15)/100;
    newsize.setHeight(newheight);
    qDebug()<<"content size:"<<newsize;
    return newsize;
}

void DGraphicsWidget::closeApplicationOnExit(bool value)
{
    if(value)
    {
    connect(this,SIGNAL(closeButtonClicked()),qApp,SLOT(closeAllWindows()));
    }
    // TODO: add diconnect also
}

void DGraphicsWidget::setCloseButtonVisible(bool value)
{
      mCloseCommandIcon->setVisible(value);
}

void DGraphicsWidget::addDefaultActions()
{
    mCloseCommandIcon =
            new DGraphicsPixmapWidget(QPixmap(":/resource/images/close.png"));
    mCommandLayout->addItem(mCloseCommandIcon);
    connect(mCloseCommandIcon,SIGNAL(triggered()),this,SIGNAL(closeButtonClicked()));
}

void DGraphicsWidget::prepareWidget()
{
#ifndef Q_OS_SYMBIAN
    resize(400,400);
#endif
    setAttribute(Qt::WA_OpaquePaintEvent,true);
    QPalette p = palette();
    p.setColor(QPalette::Base,Qt::gray);
    setPalette(p);
    setTitlePixmap(QPixmap(":/resource/images/qt.png"),true);
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
    content->setPreferredSize(contentWidgetSize());
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
