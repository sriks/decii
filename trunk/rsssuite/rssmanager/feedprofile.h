#ifndef FEEDPROFILE_H
#define FEEDPROFILE_H
#include <QObject>
#include <QTimer>
#include "rssmanager.h"

class QNetworkAccessManager;
class QNetworkReply;
class RSSParser;

/**
  Internal class for handling feeds
  */
class FeedProfile : public QObject
{
    Q_OBJECT
public:
    explicit FeedProfile(FeedSubscription subscription,QObject *parent = 0);
    bool isValid(){return !mSubscription.sourceUrl().toString().isEmpty();}
signals:
    void updateAvailable(RSSParser* parser, int updatedItems);
    void error(QString errorDescription);
public slots:
    FeedSubscription subscription(){return mSubscription;}
    void changeTimer(int mins);
    QString lastestItemTitle(){return mLatestElementTitle;}


private slots:
    void handleTimeOut();
    void replyFinished(QNetworkReply* reply);
    void handleContent(QByteArray content);
private:
    FeedSubscription mSubscription;
    QString mLatestElementTitle;
    QTimer mTimer;
    QNetworkAccessManager* mNetworkManager;
};

#endif // FEEDPROFILE_H
