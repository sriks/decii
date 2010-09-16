#include <QDebug>
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
}

void test_rssmanager::testAll()
{
    // add subscription
    for(int i=0;i<mSourceList.count();i++)
    {
         mRSSManager->addSubscription(FeedSubscription(mSourceList[i],1));
    }
    testUpdateAll();
        testUpdateAll();
            testUpdateAll();
                testUpdateAll();
                    testUpdateAll();
                        testUpdateAll();


    listAllSubscriptions();
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
        qDebug()<<parser->channelElement(RSSParser::title);
        qDebug()<<parser->itemElement(1,RSSParser::title);
    }

    parser->deleteLater();
    testUpdateAll();
    testUpdateAll();
    testUpdateAll();
        testUpdateAll();
            testUpdateAll();
                testUpdateAll();
                    testUpdateAll();
                        testUpdateAll();
                        testUpdateAll();
                            testUpdateAll();
                                testUpdateAll();
                                    testUpdateAll();
                                        testUpdateAll();
                                            testUpdateAll();
                                            testUpdateAll();
                                                testUpdateAll();
                                                    testUpdateAll();
                                                        testUpdateAll();
                                                            testUpdateAll();
                                                                testUpdateAll();
                                                                testUpdateAll();
                                                                    testUpdateAll();
                                                                        testUpdateAll();
                                                                            testUpdateAll();
                                                                                testUpdateAll();
                                                                                    testUpdateAll();

}

// eof
