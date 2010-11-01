#include <QGraphicsAnchorLayout>
#include <QGraphicsAnchor>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QBoxLayout>
#include <QFont>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QDebug>
#include "QtScrollWheel"

#include "dgraphicstextwidget.h"
#include "dcadataentrywidget.h"

const int KDefaultLDLValue  = 120;
DCADataEntryWidget::DCADataEntryWidget(QWidget* parent)
                   :QWidget(parent)
{
    QSize mScreenSize(QDesktopWidget().screenGeometry().width(),
                      QDesktopWidget().screenGeometry().height());

    mTitle = new QLabel;
    QFont font = QApplication::font();
    int fontSize = (4*mScreenSize.height())/100;
    font.setPixelSize(fontSize);
    font.setUnderline(true);
    mTitle->setFont(font);
    font = QApplication::font();
    mTitle->resize(mScreenSize.width(),mTitle->fontMetrics().height());
    mReading = new QLabel("0",this);
    fontSize = (10*mScreenSize.height())/100;
    font.setPixelSize(fontSize);
    font.setBold(true);
    mReading->setFont(font);
    font.setBold(false);
    mUnits = new QLabel("mg/dl",this);
    mSeverityIndicator = new QLabel("Low Risk");
    fontSize = (4*mScreenSize.height())/100;
    font.setPixelSize(fontSize);
    mSeverityIndicator->setFont(font);
    mQtScrollWheel = new QtScrollWheel;
    mQtScrollWheel->setSkin("Beryl");
    mQtScrollWheel->setMinimum(0);
    mQtScrollWheel->setMaximum(999);
    mQtScrollWheel->setSingleStep(1);
    mQtScrollWheel->setHorizontal(true);
    connect(mQtScrollWheel,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));

    // Create layouts
    mMasterLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    mMasterReadingLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    mTitleLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    mReadingLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    mDataEntryLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    mTitleLayout->addWidget(mTitle,0,Qt::AlignCenter);
    mReadingLayout->addWidget(mReading,0,Qt::AlignCenter);
    mReadingLayout->addWidget(mUnits,0,Qt::AlignCenter);
    mReadingLayout->setSpacing(0);
    mMasterReadingLayout->addItem(mTitleLayout);
    mMasterReadingLayout->addItem(mReadingLayout);
    mMasterReadingLayout->addWidget(mSeverityIndicator,0,Qt::AlignCenter);
    mMasterReadingLayout->setSpacing(0);
    mDataEntryLayout->addWidget(mQtScrollWheel,0,Qt::AlignCenter);
    mDataEntryLayout->addWidget(new QLabel("Slide to change value"),0,Qt::AlignCenter);
    mMasterLayout->addItem(mMasterReadingLayout);
    mMasterLayout->addItem(mDataEntryLayout);
    setLayout(mMasterLayout);
}

DCADataEntryWidget::~DCADataEntryWidget()
{
    mQtScrollWheel->deleteLater();
}

void DCADataEntryWidget::setTitle(QString title)
{
    mTitle->setText(title);
}

QString DCADataEntryWidget::title()
{
    mTitle->text();
}

void DCADataEntryWidget::setInitialValue(int initialValue)
{
    mQtScrollWheel->setValue(initialValue);
}

int DCADataEntryWidget::readingValue()
{
    return mQtScrollWheel->value();
}

void DCADataEntryWidget::valueChanged(int value)
{
    mReading->setText(QString().setNum(value));
}

void DCADataEntryWidget::resizeEvent(QResizeEvent *event)
{
    qDebug()<<event->size().width();
    qDebug()<<event->size().height();
    // landscape mode
    if(event->size().width() >= event->size().height())
    {
        mMasterLayout->setDirection(QBoxLayout::LeftToRight);
    }

    else
    {
        mMasterLayout->setDirection(QBoxLayout::TopToBottom);
    }
    updateGeometry();
    update();
}


// eof
