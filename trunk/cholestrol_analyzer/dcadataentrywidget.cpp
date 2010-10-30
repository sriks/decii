#include <QGraphicsAnchorLayout>
#include <QGraphicsAnchor>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QFont>
#include <QApplication>
#include <QGraphicsSceneResizeEvent>
#include <QDebug>
#include <QPainter>
#include "QtScrollWheel"
#include "dgraphicstextwidget.h"
#include "dcadataentrywidget.h"

const QString KScrollWheel("scrollwheel");
const QString KProxySlider("proxyslider");
DCADataEntryWidget::DCADataEntryWidget(QGraphicsItem* parent)
                   :QGraphicsWidget(parent)
{
    resize(360,640-200);
mTitle = new DGraphicsTextWidget("");
    mTitle->textItem()->setPlainText("LDL : Bad cholestrol");
    QFontMetrics fontMetrics(mTitle->textItem()->font());
    int width = fontMetrics.width(mTitle->textItem()->toPlainText());
    qDebug()<<"Width:"<<width;
    mTitle->resize(10,mTitle->size().height());
    QGraphicsLinearLayout* l = new QGraphicsLinearLayout(Qt::Vertical);
    l->addItem(mTitle);
    l->setAlignment(mTitle,Qt::AlignHCenter);
    setLayout(l);


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
    mQtScrollWheel->setMinimum(0);
    mQtScrollWheel->setMaximum(999);
    mQtScrollWheel->setSingleStep(1);
    connect(mQtScrollWheel,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
    mQtScrollWheel->setValue(250); // initial value
    mProxySlider = new QGraphicsProxyWidget;
    mProxySlider->setObjectName(KProxySlider);
    mProxySlider->setWidget(mQtScrollWheel);
    mProxySlider->installEventFilter(this);
    mQtScrollWheel->installEventFilter(this);
    mQtScrollWheel->setHorizontal(true);
    mProxySlider->setRotation(270);

//     Create layouts
    QGraphicsAnchorLayout* masterLayout = new QGraphicsAnchorLayout(this);
    QGraphicsLinearLayout* titleLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    QGraphicsLinearLayout* dataEntryLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    QGraphicsLinearLayout* readingLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    QGraphicsLinearLayout* masterReadingLayout = new QGraphicsLinearLayout(Qt::Vertical);

    Qt::Alignment alignment = Qt::AlignHCenter;


    titleLayout->addItem(mTitle);
    titleLayout->setAlignment(mTitle,alignment);
    readingLayout->setSpacing(0.5);
    readingLayout->addItem(mReading);
    readingLayout->setAlignment(mReading,alignment);
    readingLayout->addItem(mUnits);
    readingLayout->setAlignment(mUnits,alignment);
    readingLayout->setStretchFactor(mReading,2);
    dataEntryLayout->addItem(mProxySlider);
    dataEntryLayout->setAlignment(mProxySlider,alignment);

    masterReadingLayout->addItem(titleLayout);
    masterReadingLayout->setAlignment(titleLayout,alignment);
    masterReadingLayout->addItem(readingLayout);
    masterReadingLayout->setAlignment(readingLayout,alignment);
    masterReadingLayout->addItem(mSeverityIndicator);
    masterReadingLayout->setAlignment(mSeverityIndicator,alignment);
    masterLayout->setSpacing(0);

    QGraphicsAnchor* anchor = masterLayout->addAnchor(masterReadingLayout,Qt::AnchorTop,masterLayout,Qt::AnchorTop);
    anchor->setSpacing(0);
    anchor = masterLayout->addAnchor(dataEntryLayout,Qt::AnchorTop,masterReadingLayout,Qt::AnchorBottom);
    anchor->setSpacing(mProxySlider->widget()->height()/2);
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
    qDebug()<<value;
    mReading->textItem()->setPlainText(QString().setNum(value));
}

bool DCADataEntryWidget::eventFilter(QObject *obj, QEvent *event)
{
    // Once mouse is grabbed, all events are from widget
    // Hence we need to handle widget event to release the mouse
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
//    mTitle->resize(event->newSize().width(),mTitle->size().height());
}

void DCADataEntryWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}



// eof
