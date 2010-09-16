#include <QDebug>
#include <QMetaType>
#include <QXmlStreamWriter>
#include <QFile>
#include "rssmanager.h"
#include "feedprofile.h"

// Internal class to externalize and internalize feed subscription preferences
class FeedStorage
{
public:
    FeedSubscription mFeedSubscription;
    QString mLatestItemTitle;
};
const QString KFeedStorageFileName("feedstore.xml");
const QString KFeedStorageFeedElement("feed");
const QString KFeedStorageSourceUrl("sourceurl");
const QString KFeedStorageUpdatePeriod("updateperiod");
const QString KFeedStorageLatestItemTitle("latestitemtitle");

const QString KOrganisationName("decii");
const QString KAppName("rssmanager");
const QString KFeedGroup("feeds");
const QString KFeedList("feedlist");

RSSManager::RSSManager(QObject *parent) :
    QObject(parent)
{
    // read all preferences
    //internalize();
}

RSSManager::~RSSManager()
{
    // write all preferences
    qDebug()<<__FUNCTION__;
    externalize();
}

bool RSSManager::externalize()
{
    // Write all subscriptions to persistent storage as XML
    QFile storageFile(KFeedStorageFileName, this);
    if(storageFile.open(QIODevice::WriteOnly))
    {
    QXmlStreamWriter xmlWriter(&storageFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    QList<FeedProfile*>  profiles = mFeedProfiles.values();
        for(int i=0;i<profiles.count();i++)
        {
            xmlWriter.writeStartElement(KFeedStorageFeedElement);
            xmlWriter.writeTextElement(KFeedStorageSourceUrl,
                                       profiles.at(i)->subscription().sourceUrl().toString());
            xmlWriter.writeTextElement(KFeedStorageUpdatePeriod,
                                       QString().setNum(profiles.at(i)->subscription().updateInterval()));
            xmlWriter.writeTextElement(KFeedStorageLatestItemTitle,
                                       profiles.at(i)->lastestItemTitle());
            xmlWriter.writeEndElement();
        }
    xmlWriter.writeEndDocument();
    storageFile.close();
    return true;
    }

    // file not opened
    else
    {
        return false;
    }
}

bool RSSManager::internalize()
{
    return true;
}

void RSSManager::addSubscription(FeedSubscription newSubscription)
{
    FeedProfile* profile = new FeedProfile(newSubscription,this);
    connect(profile,SIGNAL(updateAvailable(QUrl,int)),this,SIGNAL(updateAvailable(QUrl,int)));
    mFeedProfiles.insert(newSubscription.sourceUrl().toString(),profile);
}

bool RSSManager::changeSubscriptionInterval(QUrl sourceUrl, int newUpdateIntervalInMins)
{
    if(newUpdateIntervalInMins <= 0)
    {return false;}

    // Get profile identified by url
    FeedProfile defaultProfile(FeedSubscription(QUrl(),-1));
    FeedProfile* profile = mFeedProfiles.value(sourceUrl.toString(),&defaultProfile);
    if(profile->isValid())
    {
        profile->changeTimer(newUpdateIntervalInMins);
        return true;
    }

// no valid feed found with the url
return false;
}

bool RSSManager::removeSubscription(QUrl sourceUrl)
{
    mFeedProfiles[sourceUrl.toString()]->deleteLater();
    return (mFeedProfiles.remove(sourceUrl.toString()) ? (true) : (false) );
}

FeedSubscription RSSManager::subscription(QUrl sourceUrl)
{
    FeedProfile defaultProfile(FeedSubscription(QUrl(),-1));
    FeedProfile* profile = mFeedProfiles.value(sourceUrl.toString(),&defaultProfile);
    return profile->subscription();
}

QList<FeedSubscription> RSSManager::subscriptions()
{
    QList<FeedSubscription> subscriptionList;
    QList<FeedProfile*>  profiles = mFeedProfiles.values();
    for(int i=0;i<profiles.count();i++)
    {
        subscriptionList.append(profiles.at(i)->subscription());
    }

    return subscriptionList;
}

QList<QUrl> RSSManager::subscriptionUrlList()
{
    QList<QUrl> urlList;
    QList<FeedProfile*>  profiles = mFeedProfiles.values();
    for(int i=0;i<profiles.count();i++)
    {
    urlList.append(profiles[i]->subscription().sourceUrl());
    }
return urlList;
}

void RSSManager::updateAll()
{
    qDebug()<<__FUNCTION__;
    QList<QString> keys = mFeedProfiles.uniqueKeys();
    for(int i=0;i<keys.count();i++)
    {
        update(QUrl(keys[i]));
    }
}

void RSSManager::update(QUrl sourceUrl)
{
    qDebug()<<__FUNCTION__;
    qDebug()<<sourceUrl;
    QString key = sourceUrl.toString();
    if(mFeedProfiles.contains(key))
    {
        mFeedProfiles[key]->update();
    }
}

RSSParser* RSSManager::parser(QUrl sourceUrl)
{
    FeedProfile defaultProfile(FeedSubscription(QUrl(),-1));
    RSSParser* parser =  mFeedProfiles.value(sourceUrl.toString(),&defaultProfile)->parser();
    return parser;
}

// end of file
