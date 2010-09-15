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
    // Check if this profile exists

    FeedProfile* profile = new FeedProfile(newSubscription,this);
    connect(profile,SIGNAL(updateAvailable(RSSParser*,int)),this,SIGNAL(updateAvailable(RSSParser*,int)));
    mFeedProfiles.insert(newSubscription.sourceUrl().toString(),profile);
}

void RSSManager::changeSubscription(QUrl sourceUrl, int newUpdateIntervalInMins)
{
    if(newUpdateIntervalInMins <= 0)
    {return;}

    // Get profile identified by url
    FeedProfile defaultProfile(FeedSubscription(QUrl(),-1));
    FeedProfile* profile = mFeedProfiles.value(sourceUrl.toString(),&defaultProfile);
    if(profile->isValid())
    {
        profile->changeTimer(newUpdateIntervalInMins);
    }

}

bool RSSManager::removeSubscription(QUrl sourceUrl)
{
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

// end of file
