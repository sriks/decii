#include <QDebug>
#include "historyengine.h"
#include "rssmanager.h"
#include "rssparser.h"
const QUrl KTDIHUrl("http://www.history.com/this-day-in-history/rss");

HistoryEngine::HistoryEngine(QObject *parent) :
               QObject(parent),
               mRSSManager(NULL)
{
    mRSSManager = new RSSManager(this);
    qDebug()<<"connecting to rssmanager:"<<
            connect(mRSSManager,SIGNAL(updateAvailable(QUrl,int)),
            this,SLOT(handleUpdateAvailable(QUrl,int)));
    // add subscription
    mRSSManager->addFeed(FeedSubscription(KTDIHUrl));
}

void HistoryEngine::handleUpdateAvailable(QUrl sourceUrl, int updateditems)
{
qDebug()<<__PRETTY_FUNCTION__;
qDebug()<<"URL:"<<sourceUrl<<"\n"<<updateditems;
    // Retrieve item
    RSSParser* parser = mRSSManager->parser(sourceUrl); // ownership is transfered
    mHistoryInfo.title = RSSParser::decodeHtml(parser->itemElement(1,RSSParser::title));
    mHistoryInfo.description = RSSParser::decodeHtml(parser->itemElement(1,RSSParser::description));
    mHistoryInfo.link = parser->itemElement(1,RSSParser::link);
    mHistoryInfo.eventDate = parser->itemElement(1,"eventDate");

// test
qDebug()<<parser->channelElement("title");
//test

    // delete parser
    parser->deleteLater();

    qDebug()<<mHistoryInfo.title<<"\n"<<
              mHistoryInfo.description<<"\n"<<
              mHistoryInfo.link<<"\n"<<
              mHistoryInfo.eventDate;

    emit updateReady(mHistoryInfo);
}

void HistoryEngine::update()
{
qDebug()<<__PRETTY_FUNCTION__;
    if(mRSSManager)
    {
#ifdef SCREENSAVER
        mRSSManager->start(KTDIHUrl);
#else
        mRSSManager->update(KTDIHUrl);
#endif
    }
}

// eof
