#include <QDebug>
#include <QFile>
#include <QDataStream>
#include "test_rssmanager.h"

const QUrl engadgeturl("http://mobile.engadget.com/rss.xml");

test_rssmanager::test_rssmanager(QObject *parent) :
    QObject(parent)
{

}

test_rssmanager::~test_rssmanager()
{
    qDebug()<<__FUNCTION__;
    delete mRSSManager;
}

void test_rssmanager::initTestCase()
{
    mRSSManager = new RSSManager(this);
    qDebug()<<connect(mRSSManager,SIGNAL(updateAvailable(QUrl,int)),this,SLOT(handleUpdateAvailable(QUrl,int)));
    mSourceList.append(QUrl("http://labs.qt.nokia.com/feed/"));
    mSourceList.append(QUrl("http://mobile.engadget.com/rss.xml"));
    mSourceList.append(QUrl("http://feeds.feedburner.com/Symbian/Blog"));
    mSourceList.append(QUrl("http://rss.allaboutsymbian.com/news/rss2all.xml"));
}

void test_rssmanager::testAll()
{
    // add subscription
    for(int i=0;i<mSourceList.count();i++)
    {
         mRSSManager->addSubscription(FeedSubscription(mSourceList[i],1));
    }
    listAllSubscriptions();
    testStop();
}

void test_rssmanager::testUpdate()
{
    // update a feed
    mRSSManager->update(mSourceList[0]);
}

void test_rssmanager::testUpdateAll()
{
    static int count = 0;
    count++;
    mRSSManager->updateAll();
    qDebug()<<__FUNCTION__<<" "<<count;
}

void test_rssmanager::testStop()
{
    qDebug()<<__FUNCTION__;
    qDebug()<<"Stopping "<<mSourceList[0];
    mRSSManager->stop(mSourceList[0]);
}

void test_rssmanager::testRestart()
{
    qDebug()<<__FUNCTION__;
    qDebug()<<"restarting "<<mSourceList[0];
    mRSSManager->restart(mSourceList[0]);
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


void test_rssmanager::testRemoveSubscriptions()
{
    // delete all subscriptions
    QList<FeedSubscription> subList = mRSSManager->subscriptions();
    mRSSManager->removeSubscription(subList[0].sourceUrl());

//    for(int i = 0;i<subList.count();i++)
//    {
//    mRSSManager->removeSubscription(subList[i].sourceUrl());
//    }

    qDebug()<<"After removing subscriptions";
    listAllSubscriptions();
}

void test_rssmanager::handleUpdateAvailable(QUrl sourceUrl, int updateditems)
{
    qDebug()<<__FUNCTION__;
    qDebug()<<"updated items:"<<updateditems;
    qDebug()<<sourceUrl;

    RSSParser* parser = mRSSManager->parser(sourceUrl);
    if(parser->isValid())
    {
        QFile outputfile("result.txt");
        qDebug()<<"opening resutlfile:"<<outputfile.open(QIODevice::WriteOnly|QIODevice::Text);
        //QDataStream stream(&outputfile);
        for(int i=1;i<=updateditems;i++)
        {
outputfile.write("\n--------------------------------------------------------------------");
        outputfile.write(parser->channelElement(RSSParser::title).toUtf8());
        outputfile.write(parser->itemElement(i,RSSParser::title).toUtf8());
        outputfile.write(parser->itemElement(i,RSSParser::pubDate).toUtf8());
        //outputfile.write(parser->category(i));
outputfile.write("--------------------------------------------------------------------\n");
        }
        outputfile.close();
    }
    parser->deleteLater();
    testRestart();
}

// eof
