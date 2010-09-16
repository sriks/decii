#include <QTimer>
#include <QHash>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QBuffer>
#include <QFile>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include "rssmanager.h"
#include "feedprofile.h"
#include "rssparser.h"

const int KOneMinInMSec = 60000;
QMutex mutex;

FeedProfile::FeedProfile(FeedSubscription subscription,QObject *parent) :
    QObject(parent),
    mSubscription(subscription)

{
    mNetworkManDestroyed = false;
    setNetworkRequestActive(false);
    connect(&mTimer,SIGNAL(timeout()),this,SLOT(handleTimeOut()));
    mNetworkManager = NULL;
    changeTimer(mSubscription.updateInterval());
    // start initial fetch
    if(mSubscription.updateInterval())
    { update(); }
}

FeedProfile::~FeedProfile()
{
    mTimer.stop();
    delete mNetworkManager;
    mNetworkManager = NULL;
}

RSSParser* FeedProfile::parser()
{
    qDebug()<<__FUNCTION__;
    RSSParser* parser = new RSSParser;
    QFile* feedFile = new QFile(feedFileName(),parser);
    if(feedFile->open(QIODevice::ReadOnly))
    {
        parser->setSource(feedFile);
    }
    return parser;
}

void FeedProfile::update()
{
    // ignore this if a request is already active
    if(!isNetworkRequestActive())
    handleTimeOut();
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

    // ignore
    if(isNetworkRequestActive()) { return; }

    // Fetch feed from source
    if(mNetworkManDestroyed)
    {
    mNetworkManager = new QNetworkAccessManager(this);
    mNetworkManDestroyed = false;
    }
    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
    connect(mNetworkManager,SIGNAL(destroyed(QObject*)),this,SLOT(handleNetworkMgrDestroyed(QObject*)));
    mNetworkManager->get(QNetworkRequest(mSubscription.sourceUrl()));
    setNetworkRequestActive(true);
}

void FeedProfile::replyFinished(QNetworkReply *reply)
{
    qDebug()<<__FUNCTION__;
    QMutex m;
    m.lock();
        setNetworkRequestActive(false);
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
                emit error(reply->errorString());
            }
        reply->deleteLater();
        // Feeds are usually gathered in periodic intervals.
        // So network manager need not reside in memory till it is required.
        mNetworkManager->deleteLater();
    m.unlock();
}

void FeedProfile::handleContent(QByteArray content)
{
    qDebug()<<__FUNCTION__;
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
qDebug()<<"leaving "<<__FUNCTION__;
}

QString FeedProfile::feedFileName()
{
    QString filename;
    filename.setNum( qHash(mSubscription.sourceUrl().toString()) );
    filename.append(".xml");
    return filename;
}

// test slot
void FeedProfile::handleNetworkMgrDestroyed(QObject *obj)
{
    qDebug()<<__FUNCTION__;
    QMutex m;
    m.lock();
        if(obj)
        {
            obj->objectName();
        }
        mNetworkManDestroyed = true;
    m.unlock();
}

void FeedProfile::setNetworkRequestActive(bool value)
{
    QMutex m;
    m.lock();
    mNetworkRequestActive = value;
    m.unlock();
}

bool FeedProfile::isNetworkRequestActive()
{
    return mNetworkRequestActive;
}

// eof
