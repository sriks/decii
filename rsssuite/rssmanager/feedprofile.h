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
    ~FeedProfile();
    bool isValid(){return !mSubscription.sourceUrl().toString().isEmpty();}
    RSSParser* parser();
signals:
    void updateAvailable(QUrl sourceUrl, int updatedItems);
    void error(QString errorDescription);

public slots:
    FeedSubscription subscription(){return mSubscription;}

    /** \param mins a value less than zero stops the timer
     **/
    void changeTimer(int mins);
    QString lastestItemTitle(){return mLatestElementTitle;}


private slots:
    void handleTimeOut();
    void replyFinished(QNetworkReply* reply);
    void handleContent(QByteArray content);

    // test slots
    void handleDestroyed(QObject* obj);
private:
    QString feedFileName();
private:
    FeedSubscription mSubscription;
    QString mLatestElementTitle;
    QString mFeedFileName;
    QTimer mTimer;
    QNetworkAccessManager* mNetworkManager;
};

#endif // FEEDPROFILE_H
