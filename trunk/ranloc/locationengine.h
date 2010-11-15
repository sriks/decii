#ifndef LocationEngine_H
#define LocationEngine_H
#include <QObject>
#include <QUrl>

#ifdef Q_OS_SYMBIAN
#define BUG_8687
#endif

class LocationDetails : public QObject
{
    Q_OBJECT;
public:
    LocationDetails(QObject* parent = 0)
        :QObject(parent)
    {
    }

    QString title;
    QString summary;
    QString feature;
    QString countryCode;
    QString latitude;
    QString longitude;
    QUrl imageLink;
    QUrl link;
};

#ifdef BUG_8687
class S60QHttp;
#endif
class QNetworkAccessManager;
class QNetworkReply;
class QIODevice;
class LocationEngine : public QObject
{
    Q_OBJECT
    Q_ENUMS(EntryElement)

public:
    enum EntryElement
    {
        lang,
        title,
        summary,
        feature,
        countryCode,
        lat,
        lng,
        wikipediaUrl,
        thumbnailImg
    };


    LocationEngine(QObject* parent = 0);
    ~LocationEngine();
    void newLocation(QUrl url);
    LocationDetails* nextLocation();
    LocationDetails* currentLocation(){return mCurrentLocationDetails;}
    int count(){return mCount;}
    int currentIndex(){return mCurrentIndex;}


signals:
    void newLocationAvailable();
    void errorOccured(QString error);

protected:
    QString readElement(QIODevice* xmlSource,int index,EntryElement element);
    QString executeQuery(QIODevice* xmlSource, const QString& aQuery);

protected slots:
    #ifdef BUG_8687
    void on_http_requestFinished (int id,bool error);
    #else
    void replyFinished(QNetworkReply *reply);
    #endif
    void handleContent(QByteArray content);

private:
#ifdef BUG_8687
    S60QHttp* mHttp;
#else
    QNetworkAccessManager* mNetworkManager;
#endif
    int mCurrentIndex;
    LocationDetails* mCurrentLocationDetails;
    int mCount;
};

#endif // LocationEngine_H
