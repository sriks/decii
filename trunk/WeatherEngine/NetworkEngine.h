#ifndef NETWORKENGINE_H
#define NETWORKENGINE_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>

class NetworkEngine : public QObject
{
    Q_OBJECT
public:
    explicit NetworkEngine(QObject *parent = 0);
public slots:
    //void post(QNetworkRequest* request);
    void get(QNetworkRequest& request);
private slots:
    void replyAvailable(QNetworkReply* reply);
signals:
    void dataAvailable(QByteArray data);
    void error(QNetworkReply::NetworkError errorType,QString errorString);
private:
    QNetworkAccessManager* nam;
};

#endif // NETWORKENGINE_H
