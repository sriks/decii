#include <QDebug>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QListWidget>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rssmanager.h"
#include "rssparser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
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
    qDebug()<<sourceUrl<<"\nupdates:"<<updateditems;

    mCurrentNotificationUrl.clear();
    if(updateditems)
    {
        RSSParser* parser = mRSSManager->parser(sourceUrl);
        if(parser->isValid())
        {

            ui->listWidget->clear();
            QStringList titleList = parser->itemElements(RSSParser::title);
            for(int i=0;i<titleList.count();i++)
            {
                titleList[i] = parser->decodeHtml(titleList[i]);
            }

            ui->listWidget->addItems(titleList);

//            connect(ui->listWidget,SIGNAL(itemActivated(QListWidgetItem*)))
        }
    }
}

void MainWindow::addFeedUrl()
{
    QUrl url;
    url.setUrl(ui->urleditor->text());
    if(url.isValid())
    {
        mRSSManager->addFeed(FeedSubscription(url,1));
        mRSSManager->start(url);
        ui->urlListWidget->addItem(url.toString());
    }
}

void MainWindow::handleItemActivated(QListWidgetItem* item)
{
    // open with default app
//    QDesktopServices::openUrl();
}
