#include <QDebug>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QListWidget>
#include <QGraphicsLineItem>
#include <QGraphicsLinearLayout>
#include <QGraphicsAnchorLayout>
#include <QGraphicsAnchor>
#include <QVBoxLayout>
#include <QGraphicsWebView>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rssmanager.h"
#include "rssparser.h"
#include "dgraphicswidget.h"
#include "dgraphicsview.h"
#include "ksegment.h"
#include "ksegmentwidget.h"
#include "ksegmentview.h"
#include "dgraphicstextwidget.h"
#include "feedview.h"

typedef KSegment Segment;
typedef KSegmentWidget SegmentWidget;
typedef KSegmentView SegmentView;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mFeedView = NULL;
    ui->setupUi(this);
    mTray = new QSystemTrayIcon(this);
    mTray->setContextMenu(new QMenu(this));
    mRSSManager = new RSSManager(this);
    connect(mRSSManager,SIGNAL(updateAvailable(QUrl,int)),this,SLOT(handleUpdateAvailable(QUrl,int)));
    connect(ui->add,SIGNAL(clicked()),this,SLOT(addFeedUrl()));
    connect(mTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(noticationActivated()));

//    mSourceList.append(QUrl("http://labs.qt.nokia.com/feed/"));
//    mSourceList.append(QUrl("http://mobile.engadget.com/rss.xml"));
//    mSourceList.append(QUrl("http://feeds.feedburner.com/Symbian/Blog"));
//    mSourceList.append(QUrl("http://rss.allaboutsymbian.com/news/rss2all.xml"));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mRSSManager;
}

void MainWindow::handleUpdateAvailable(QUrl sourceUrl, int updateditems)
{
    qDebug()<<__FUNCTION__;

    if(updateditems)
    {
        RSSParser* parser = mRSSManager->parser(sourceUrl);
        this->setVisible(false);
        if(parser->isValid())
        {
        if(NULL == mFeedView)
        {
            mFeedView = new FeedView();
        }
            mFeedView->showFeedView(parser);
            mFeedView->showMaximized();

//            RSSParser* aParser    = parser;
//            KSegmentView* view = new SegmentView(Qt::Vertical);
//            QFont dateFont(QApplication::font());
//            dateFont.setItalic(true);
//            dateFont.setPixelSize(8);
//            for(int i=1;i<=5;++i)
//            {
//                QGraphicsTextItem* title = new QGraphicsTextItem;
//                title->setHtml(aParser->decodeHtml(aParser->itemElement(i,RSSParser::title)));
//                title->setTextWidth(360);
////                DGraphicsTextWidget* title = new DGraphicsTextWidget(aParser->decodeHtml(aParser->itemElement(i,RSSParser::title)));
//                QGraphicsTextItem* date = new QGraphicsTextItem;
//                date->setHtml(aParser->decodeHtml(aParser->itemElement(i,RSSParser::pubDate)));
//                date->setFont(dateFont);
//                SegmentWidget* segWidget = new SegmentWidget(Qt::Vertical);
//                segWidget->addItem(title);
//                segWidget->addItem(date);
//                view->addSegmentWidget(segWidget);
//            }
//            view->showMaximized();
        }

        parser->deleteLater();
    }
}

void MainWindow::addFeedUrl()
{
    QUrl url;
    url.setUrl(ui->urleditor->text());
    if(url.isValid())
    {
        mRSSManager->addFeed(FeedSubscription(url,30));
        mRSSManager->start(url);
        ui->urlListWidget->addItem(url.toString());
    }
    this->setVisible(false);
}

void MainWindow::handleItemActivated(QListWidgetItem* item)
{
    // open with default app
//    QDesktopServices::openUrl();
}
