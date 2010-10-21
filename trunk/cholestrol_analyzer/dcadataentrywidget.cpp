#include <QGraphicsAnchorLayout>
#include <QGraphicsAnchor>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QFont>
#include <QApplication>
#include "QtScrollWheel"
#include "QtSvgButton"
#include "dgraphicstextwidget.h"
#include "dcadataentrywidget.h"

DCADataEntryWidget::DCADataEntryWidget(QGraphicsItem* parent)
                   :QGraphicsWidget(parent)
{
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
    mQtScrollWheel->setSkin("Beryl");
    mQtScrollWheel->setMinimum(0);
    mQtScrollWheel->setMaximum(999);
    mQtScrollWheel->setSingleStep(1);
    connect(mQtScrollWheel,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
    mProxySlider = new QGraphicsProxyWidget;
    mProxySlider->setWidget(mQtScrollWheel);
    mProxySlider->setFlag(QGraphicsItem::ItemIsSelectable,true);
    mProxySlider->setFlag(QGraphicsItem::ItemIsMovable,true);
    mProxyButtonOk = new QGraphicsProxyWidget;
    QtSvgButton* okButton = new QtSvgButton;
    okButton->setSkin("Beryl");
    okButton->setText("OK");
    mProxyButtonOk->setWidget(okButton);
    mProxyButtonOk->resize(50,50);


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
    buttonLayout->addItem(mProxyButtonOk);
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

// eof
