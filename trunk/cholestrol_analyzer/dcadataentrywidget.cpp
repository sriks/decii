#include <QGraphicsAnchorLayout>
#include <QGraphicsAnchor>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QFont>
#include <QApplication>
#include <QGraphicsSceneResizeEvent>
#include <QDebug>
#include "QtScrollWheel"
#include "dgraphicstextwidget.h"
#include "dcadataentrywidget.h"

const QString KScrollWheel("scrollwheel");
const QString KProxySlider("proxyslider");

DCADataEntryWidget::DCADataEntryWidget(QGraphicsItem* parent)
                   :QGraphicsWidget(parent)
{
    resize(360,640);
    mTitle = new DGraphicsTextWidget("LDL : Bad cholestrol");
    QFont font = QApplication::font();
    font.setPixelSize(12);
    font.setBold(true);
    mTitle->textItem()->setFont(font);
    mReading = new DGraphicsTextWidget("0");
    font.setPixelSize(20);
    mReading->textItem()->setFont(font);
    mUnits = new DGraphicsTextWidget("mg/dl");
    font.setPixelSize(10);
    font.setBold(false);
    mUnits->textItem()->setFont(font);
    mSeverityIndicator = new DGraphicsTextWidget("Low Risk");
    font.setPixelSize(15);
    mSeverityIndicator->textItem()->setFont(font);
    mQtScrollWheel = new QtScrollWheel;
    mQtScrollWheel->setObjectName(KScrollWheel);
    mQtScrollWheel->setSkin("Beryl");
    mQtScrollWheel->resize(QSize(150,350));
    mQtScrollWheel->setMinimum(0);
    mQtScrollWheel->setMaximum(999);
    mQtScrollWheel->setSingleStep(1);
    connect(mQtScrollWheel,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
    mQtScrollWheel->setValue(250); // initial value
    mProxySlider = new QGraphicsProxyWidget;
    mProxySlider->setObjectName(KProxySlider);
    mProxySlider->setWidget(mQtScrollWheel);
    mProxySlider->setFlag(QGraphicsItem::ItemIsSelectable,true);
    mProxySlider->setFlag(QGraphicsItem::ItemIsMovable,true);
    mProxySlider->installEventFilter(this);
    mQtScrollWheel->installEventFilter(this);
    mProxySlider->rotate(90);
    // Create layouts
    QGraphicsAnchorLayout* masterLayout = new QGraphicsAnchorLayout(this);
    QGraphicsLinearLayout* titleLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    QGraphicsLinearLayout* dataEntryLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    QGraphicsLinearLayout* dataEntryLayoutLeft = new QGraphicsLinearLayout(Qt::Vertical);
    QGraphicsLinearLayout* readingLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    QGraphicsLinearLayout* buttonLayout = new QGraphicsLinearLayout(Qt::Horizontal);

    titleLayout->addItem(mTitle);
    readingLayout->addItem(mReading);
    readingLayout->addItem(mUnits);
    readingLayout->setStretchFactor(mReading,2);
    dataEntryLayoutLeft->addItem(readingLayout);
    dataEntryLayoutLeft->addItem(mSeverityIndicator);
    dataEntryLayoutLeft->addItem(buttonLayout);
    dataEntryLayout->addItem(dataEntryLayoutLeft);
    dataEntryLayout->addItem(mProxySlider);
    dataEntryLayout->setStretchFactor(mProxySlider,1);

    QGraphicsAnchor* anchor = masterLayout->addAnchor(titleLayout,Qt::AnchorTop,masterLayout,Qt::AnchorTop);
    masterLayout->addCornerAnchors(dataEntryLayout,Qt::TopLeftCorner,titleLayout,Qt::BottomLeftCorner);
    masterLayout->addCornerAnchors(dataEntryLayout,Qt::TopRightCorner,titleLayout,Qt::BottomRightCorner);
    masterLayout->addCornerAnchors(dataEntryLayout,Qt::BottomRightCorner,masterLayout,Qt::BottomRightCorner);
    setLayout(masterLayout);

}

DCADataEntryWidget::~DCADataEntryWidget()
{
    mQtScrollWheel->deleteLater();
}

void DCADataEntryWidget::setTitle(QString title)
{
    mTitle->textItem()->setPlainText(title);
}

QString DCADataEntryWidget::title()
{
    mTitle->textItem()->toPlainText();
}

void DCADataEntryWidget::valueChanged(int value)
{
    mReading->textItem()->setPlainText(QString().setNum(value));
}

bool DCADataEntryWidget::eventFilter(QObject *obj, QEvent *event)
{
    // Once mouse is grabbed, all events are from widget
    // Hence we need to handle widget event to release the mouse

    qDebug()<<event->type();
    if(QEvent::GraphicsSceneMousePress == event->type() &&
       KProxySlider == obj->objectName())
    {
        qDebug()<<"Grabbing mouse";
        mProxySlider->widget()->grabMouse();
    }

    else if(QEvent::MouseButtonRelease == event->type() &&
            KScrollWheel == obj->objectName())
    {
        qDebug()<<"Releasing mouse";
        mProxySlider->widget()->releaseMouse();
    }

    // TODO: If pressed and moved outside slider area, it can still move
    //       Fix is to handle hover events on widget and behave
    return false;
}

void DCADataEntryWidget::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    qDebug()<<event->oldSize();
    qDebug()<<event->newSize();
    resize(event->newSize());

}
// eof
