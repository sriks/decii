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
    explicit FeedSubscription(QUrl source,int updateIntervalInMins = 60)
    {
        mSourceUrl = source;
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
    void addSubscription(FeedSubscription newSubscription);
    bool changeSubscriptionInterval(QUrl sourceUrl, int newUpdateIntervalInMins);

// TODO: add stop updates support
//    void stopUpdates(QUrl sourceUrl);
//    void stopAllUpdates();

    bool removeSubscription(QUrl sourceUrl);
    FeedSubscription subscription(QUrl sourceUrl);
    QList<FeedSubscription> subscriptions();
    int subscriptionCount(){return mFeedProfiles.count();}

signals:
    // signal that new item is available
    // Its is responsibility of caller to delete parser. Use deleteLater()
    void updateAvailable(RSSParser* parser, int updateditems);

private slots:
    bool externalize();
    bool internalize();

private:
    int toMSec(int minutes);
private:
    QHash<QString,FeedProfile*> mFeedProfiles;
};
#endif // RSSMANAGER_H
