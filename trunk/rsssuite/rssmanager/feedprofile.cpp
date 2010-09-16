#include <QTimer>
#include <QHash>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QBuffer>
#include <QFile>
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
    changeTimer(mSubscription.updateInterval());
    // start initial fetch
    if(mSubscription.updateInterval())
    { handleTimeOut(); }
}

FeedProfile::~FeedProfile()
{
    mTimer.stop();
    delete mNetworkManager;
    mNetworkManager = NULL;
}

RSSParser* FeedProfile::parser()
{
    RSSParser* parser = new RSSParser;
    QFile* feedFile = new QFile(feedFileName(),parser);
    if(feedFile->open(QIODevice::ReadOnly))
    {
        parser->setSource(feedFile);
    }
    return parser;
}

void FeedProfile::changeTimer(int mins)
{
    mTimer.stop();

    // start timer if it is a valid interval
    if(mins>=0)
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
            QFile feedFile(feedFileName());
            if(feedFile.exists())
            {
                feedFile.remove();
            }

            if(feedFile.open(QIODevice::ReadWrite))
            {
                QString tmp(content);
                feedFile.write(tmp.toUtf8());
                feedFile.close();
            }

            else
            {
                emit error("Cannot store the feed");
                return;
            }

            int newItemsCount=0;
            QFile readFeedFile(feedFileName());
            qDebug()<<readFeedFile.open(QIODevice::ReadOnly);
            RSSParser* parser = new RSSParser(this);
            parser->setSource(&readFeedFile);
            QStringList titles = parser->itemElements(RSSParser::title);
            parser->deleteLater();
            readFeedFile.close();

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
                    emit updateAvailable(mSubscription.sourceUrl(),newItemsCount);
                }
            }

            // Parsing error, No items found in parsing
            else
            {
                emit error(tr("Cannot parse feed"));
            }

     }
}

QString FeedProfile::feedFileName()
{
    QString filename;
    filename.setNum( qHash(mSubscription.sourceUrl().toString()) );
    //filename.append(".xml");
    return filename;
}

void FeedProfile::handleDestroyed(QObject *obj)
{
    qDebug()<<__FUNCTION__;
    if(obj)
    {
        obj->objectName();
    }
}

// eof
