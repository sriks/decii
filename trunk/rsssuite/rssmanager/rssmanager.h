// author: srikanthsombhatla@gmail.com
#ifndef RSSMANAGER_H
#define RSSMANAGER_H
#include <QObject>
#include <QUrl>
#include <QHash>
#include "rssparser.h"


/**
  Class to hold a RSS Subscrition info
  **/
class FeedSubscription
{
public:
    explicit FeedSubscription(QUrl sourceUrl,int updateIntervalInMins = 60)
    {
        mSourceUrl = sourceUrl;
        mUpdateInterval = updateIntervalInMins;
    }

    QUrl sourceUrl(){return mSourceUrl;}
    void setSourceUrl(QUrl source){mSourceUrl = source;}
    int updateInterval(){return mUpdateInterval;}
    void setUpdateInterval(int updateIntervalInMins){mUpdateInterval = updateIntervalInMins;}

private:
    QUrl mSourceUrl;
    int mUpdateInterval;
};

// Forward declaration
class FeedProfile;
class RSSManager : public QObject
{
    Q_OBJECT
public:
    explicit RSSManager(QObject *parent = 0);
    ~RSSManager();

    // adds a subscription.
    // This doest initiate update cycle.
    // Client should call start()
    void addFeed(FeedSubscription newSubscription);

    // This changed subscription interval to new interval.
    // This does not initiate a fetch on feed.
    // Note: If a negative value is supplied, subscription is stopped.
    bool setUpdateInterval(QUrl sourceUrl, int newUpdateIntervalInMins);
    RSSParser* parser(QUrl sourceUrl);
    bool isFeedValid(QUrl sourceUrl);

    // Start subscription in defined intervals
    // It has no effect if called on a subscription which is already started or
    // a subscription which doest have a valid interval
    bool start(QUrl sourceUrl);
    void startAll();
    // stop fetching feeds
    // This has no effect if a feed is already fetched and being processed.
    bool stop(QUrl sourceUrl);
    void stopAll();

    bool removeFeed(QUrl sourceUrl);
    FeedSubscription feed(QUrl sourceUrl);
    QList<FeedSubscription> feeds();
    QList<QUrl> feedUrls();
    int feedsCount(){return mFeedProfiles.count();}

    // on demand update
    // updates even if no interval is set
    void updateAll();
    void update(QUrl sourceUrl);

signals:
    // signal that new item is available
    void updateAvailable(QUrl sourceUrl, int updateditems);

private slots:
    bool externalize();
    bool internalize();

private:
    QHash<QString,FeedProfile*> mFeedProfiles;
};
#endif // RSSMANAGER_H
