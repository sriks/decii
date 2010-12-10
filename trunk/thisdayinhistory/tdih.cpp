#include <QDebug>
#include <QApplication>
#include <QGraphicsWebView>
#include <QTextCodec>
#include <QTextDecoder>
#include <QTextDocument>
#include <QGraphicsDropShadowEffect>
#include "tdih.h"
#include "dgraphicswidget.h"
#include "dgraphicsview.h"

const QString KMainTitle("THIS DAY IN HISTORY");
TDIH::TDIH(QWidget *parent)
    : QWidget(parent),
      mUpdateAvailable(false)
{
    mDGraphicsView = new DGraphicsView;
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    mDGraphicsView->setGraphicsEffect(shadow);
    mDGraphicsWidget = new DGraphicsWidget;
    mDGraphicsWidget->setParent(mDGraphicsView);
    mDGraphicsWidget->setTitleText(KMainTitle);
    mDGraphicsWidget->closeApplicationOnExit(true);
    QFont titlefont(QApplication::font());
    mDGraphicsView->scene()->addItem(mDGraphicsWidget);
    mDTextWidget = new DGraphicsTextWidget("",mDGraphicsWidget);
    mWebContent = new QGraphicsWebView(mDGraphicsWidget);
    mDGraphicsWidget->addContent(mDTextWidget);
    mHistoryEngine = new HistoryEngine(this);
    qDebug()<<"connecting historyengine:"<<connect(mHistoryEngine,SIGNAL(updateReady(HistoryInfo)),
            this,SLOT(handleUpdate(HistoryInfo)));
    mHistoryEngine->update();
    mDGraphicsWidget->setTitleText(KMainTitle+"\nLoading...");
#ifdef Q_OS_SYMBIAN
    mDGraphicsView->showMaximized();
#else
    mDGraphicsView->show();
#endif
}

TDIH::~TDIH()
{
    delete mDGraphicsView;
}

void TDIH::handleUpdate(HistoryInfo info)
{
qDebug()<<__PRETTY_FUNCTION__;
   mHistoryInfo = info;
   mUpdateAvailable = true;
   showWidget();
}

void TDIH::showWidget()
{
qDebug()<<__PRETTY_FUNCTION__;
    if(!mUpdateAvailable)
    {
        mDTextWidget->textItem()->setPlainText(tr("Sorry, no Updates!!!"));
    }
    else
    {
        QTextDocument textdoc;
        textdoc.setHtml(mHistoryInfo.description);
        textdoc.adjustSize();
        QSize idealSize = textdoc.size().toSize();
        qDebug()<<"idealsize:"<<idealSize;
        mDGraphicsView->resize(idealSize.width(),idealSize.height()+200);
        mDGraphicsWidget->resize(idealSize.width(),idealSize.height()+100);
        mDTextWidget->textItem()->setTextWidth(idealSize.width());
        mDTextWidget->textItem()->setHtml(mHistoryInfo.description);
        QFont contentFont = mDTextWidget->font();
        contentFont.setPixelSize( /*(50*contentFont.pixelSize())/100*/ 7 );
        mDTextWidget->setFont(contentFont);
    }

#ifdef SCREENSAVER
    mDGraphicsWidget->setCloseButtonVisible(false);
    mDGraphicsView->moveToCenter();
#endif

    mDGraphicsWidget->setTitleText(KMainTitle+"\n"+mHistoryInfo.title);
    mDGraphicsView->show();
}


// eof
