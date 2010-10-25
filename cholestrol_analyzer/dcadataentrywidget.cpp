#include <QGraphicsAnchorLayout>
#include <QGraphicsAnchor>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QBoxLayout>
#include <QFont>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include "QtScrollWheel"
#include "QtSvgButton"
#include "dgraphicstextwidget.h"
#include "dcadataentrywidget.h"

const int KDefaultLDLValue  = 120;
DCADataEntryWidget::DCADataEntryWidget(QWidget* parent)
                   :QWidget(parent)
{
    QSize mScreenSize(QDesktopWidget().screenGeometry().width(),
                      QDesktopWidget().screenGeometry().height());
    mTitle = new QLabel("LDL : Bad cholestrol",this);
    QFont font = QApplication::font();
    int fontSize = (5*mScreenSize.height())/100;
    qDebug()<<"fontSize:"<<fontSize;
    font.setPixelSize(fontSize);
    mTitle->setFont(font);
    mTitle->resize(mScreenSize.width(),mTitle->fontMetrics().height());
    mReading = new QLabel("0",this);
    mUnits = new QLabel("mg/dl",this);
    mSeverityIndicator = new QLabel("Low Risk",this);
    mQtScrollWheel = new QtScrollWheel(this);
    mQtScrollWheel->setSkin("Beryl");
    mQtScrollWheel->setMinimum(0);
    mQtScrollWheel->setMaximum(999);
    mQtScrollWheel->setSingleStep(1);

    connect(mQtScrollWheel,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
    mQtScrollWheel->setValue(KDefaultLDLValue);
    int wheelWidth = (15*mScreenSize.width())/100;
    int wheelHeight = (15*mScreenSize.height())/100;

    // Create layouts
    QBoxLayout* masterLayout = new QBoxLayout(QBoxLayout::LeftToRight,this);
    QBoxLayout* titleLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    QBoxLayout* dataEntryLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    QBoxLayout* dataEntryLayoutLeft = new QBoxLayout(QBoxLayout::TopToBottom);
    QBoxLayout* readingLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    QBoxLayout* buttonLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    titleLayout->addWidget(mTitle);
    readingLayout->addWidget(mReading);
    readingLayout->addWidget(mUnits);
    readingLayout->setStretchFactor(mReading,2);
    dataEntryLayoutLeft->addItem(readingLayout);
    dataEntryLayoutLeft->addWidget(mSeverityIndicator);
//    buttonLayout->addItem(mProxyButtonOk);
    dataEntryLayoutLeft->addItem(buttonLayout);
    dataEntryLayout->addItem(dataEntryLayoutLeft);
    dataEntryLayout->addWidget(mQtScrollWheel);
    dataEntryLayout->setStretchFactor(mQtScrollWheel,1);


    masterLayout->addItem(dataEntryLayoutLeft);
    masterLayout->addWidget(mQtScrollWheel);

    setLayout(masterLayout);
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

void DCADataEntryWidget::valueChanged(int value)
{
    mReading->setText(QString().setNum(value));
}

// eof
