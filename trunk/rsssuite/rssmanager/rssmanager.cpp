#include "rssmanager.h"

struct FeedInfo
{
    QUrl sourceUrl;
    float updateInterval;
    bool matchKeys;
};

RSSManager::RSSManager(QObject *parent) :
    QObject(parent)
{
}
