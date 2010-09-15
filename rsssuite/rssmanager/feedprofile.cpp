#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QBuffer>
#include <QDebug>
#include "rssmanager.h"
#include "feedprofile.h"
#include "rssparser.h"

const int KOneMinInMSec = 60000;

FeedProfile::FeedProfile(FeedSubscription subscription,QObject *parent) :
    QObject(parent),
    mSubscription(subscription)

{
    connect(&mTimer,SIGNAL(timeout()),this,SLOT(handleTimeOut()));
    mNetworkManager = NULL;
    mTimer.setInterval(subscription.updateInterval()*KOneMinInMSec);
    mTimer.start();
    // start initial fetch
    handleTimeOut();
}

FeedProfile::~FeedProfile()
{
    mTimer.stop();
    delete mNetworkManager;
    mNetworkManager = NULL;
}

void FeedProfile::changeTimer(int mins)
{
    mTimer.stop();

    // restart timer if it is a valid interval
    if(mins)
    {
    mTimer.setInterval(mins * KOneMinInMSec);
    mTimer.start();
    }
}

void FeedProfile::handleTimeOut()
{
    qDebug()<<__FUNCTION__;
    // Fetch feed from source
    mNetworkManager = new QNetworkAccessManager(this);
    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
    mNetworkManager->get(QNetworkRequest(mSubscription.sourceUrl()));
}

void FeedProfile::replyFinished(QNetworkReply *reply)
{
    qDebug()<<__FUNCTION__;
    // No error
    if(QNetworkReply::NoError == reply->error())
    {
        // read contents
        QByteArray content = reply->readAll();
        handleContent(content);
    }

    // handle error
    else
    {

    }

    reply->deleteLater();
    // Feeds are usually gathered in periodic intervals.
    // So network manager need not reside in memory till it is required.
    mNetworkManager->deleteLater();
}

void FeedProfile::handleContent(QByteArray content)
{
    qDebug()<<__FUNCTION__;
    //qDebug()<<content;
    // valid content
    if(content.size())
    {
            int newItemsCount=0;
            RSSParser* parser = new RSSParser;
            QBuffer* buffer = new QBuffer(&content,parser); // gets deleted with parser
            buffer->open(QIODevice::ReadOnly);
            parser->setSource(buffer); // Ready for parsing

            QStringList titles = parser->itemElements(RSSParser::title);
            int totalItems = titles.count();
            if(totalItems)
            {
                // Assume all items are new
                newItemsCount = totalItems;

                // Check for updates
                if(!mLatestElementTitle.isEmpty() && (titles.indexOf(mLatestElementTitle) >= 0))
                {
                    newItemsCount = titles.indexOf(mLatestElementTitle);
                }

                // New updates available
                if(newItemsCount)
                {
                    // XQuery numbering starts with 1, so we can send count as such
                    mLatestElementTitle = titles[0];
                    // Ownership of parser is transfered
                    emit updateAvailable(parser,newItemsCount);
                }
            }

            // Parsing error, No items found in parsing
            else
            {
                emit error(tr("Cannot parse feed"));
            }

            // No updates available
            if(0 == newItemsCount)
            {
                parser->deleteLater();
            }
     }
}

// eof
