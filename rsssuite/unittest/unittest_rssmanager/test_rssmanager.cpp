#include <QDebug>
#include "test_rssmanager.h"

test_rssmanager::test_rssmanager(QObject *parent) :
    QObject(parent)
{

}

void test_rssmanager::initTestCase()
{
    mRSSManager = new RSSManager(this);
    qDebug()<<connect(mRSSManager,SIGNAL(updateAvailable(RSSParser*,int)),this,SLOT(handleUpdateAvailable(RSSParser*,int)));
}

void test_rssmanager::testAll()
{
    // add subscription
    mRSSManager->addSubscription(FeedSubscription(QUrl("http://labs.trolltech.com/blogs/feed/"),0.01));
    mRSSManager->addSubscription(FeedSubscription(QUrl("http://mobile.engadget.com/rss.xml"),0.01));
    listAllSubscriptions();
    int count = mRSSManager->subscriptionCount();
    // this tries to modify a subscription
    for(int i=0;i<count;i++)
    {
         // Get the feed uniquely identified by url
         FeedSubscription feed = mRSSManager->subscription(QUrl("http://labs.trolltech.com/blogs/feed/"));
         feed.setSourceUrl(QUrl("http://feeds.feedburner.com/TheQtBlog"));

         // Add updated feed
         mRSSManager->addSubscription(feed);
    }
    qDebug()<<"After modification";
    listAllSubscriptions();
}

void test_rssmanager::listAllSubscriptions()
{
    int subCount = mRSSManager->subscriptionCount();
    qDebug()<<"Total subscriptions:"<<subCount;
    QList<FeedSubscription> subList = mRSSManager->subscriptions();
    for(int i=0;i<subList.count();i++)
    {
        qDebug()<<"Source url:"<<subList[i].sourceUrl().toString();
        qDebug()<<"Update interval:"<<subList[i].updateInterval();
    }
}

void test_rssmanager::handleUpdateAvailable(RSSParser* parser, int updateditems)
{
    qDebug()<<__FUNCTION__;
    qDebug()<<"updated items:"<<updateditems;
    qDebug()<<parser->channelElement(RSSParser::title);
}

// eof
