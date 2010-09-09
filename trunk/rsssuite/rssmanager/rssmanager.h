#ifndef RSSMANAGER_H
#define RSSMANAGER_H
#include <QObject>

struct ItemInfo
{
    QString channelName;
    QUrl    sourceUrl;
    QString title;
    QUrl    url;
    QString date;
    QStringList tags;
};

class QUrl;
struct FeedInfo;
class RSSManager : public QObject
{
    Q_OBJECT
public:
    explicit RSSManager(QObject *parent = 0);

    // sets the path of xml
    void setSubscription(QString path);

    // reparses subscription list
    void updateSubscription();

signals:
    // signal that new item is available
    void updateAvailable(const QList<ItemInfo>& itemInfo);

private:
    bool parseFeedInfo(QString path);

private:
    QList<FeedInfo> mFeedInfoList;
};
#endif // RSSMANAGER_H
