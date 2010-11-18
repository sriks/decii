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

class QNetworkAccessManager;
class QNetworkReply;
class QIODevice;
class QXmlQuery;
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

    enum Queries
    {
        countryinfoforcountrycode,
        locationinfoforindex
    };

    LocationEngine(QObject* parent = 0);
    ~LocationEngine();
    void newLocation(QUrl url);
    LocationDetails* nextLocation();
    LocationDetails* prevLocation();
    LocationDetails* currentLocation(){return mCurrentLocationDetails;}
    int count(){return mCount;}
    int currentIndex(){return mCurrentIndex;}

signals:
    void newLocationAvailable();
    void errorOccured(QString error);

protected slots:
    void replyFinished(QNetworkReply *reply);
    void handleContent(QByteArray content);

private:
    QString readElement(QIODevice* xmlSource,int index,EntryElement element);
    QString executeQuery(QIODevice* xmlSource, const QString& aQuery);
    LocationDetails* locationDetails(int index);

    QString query();
private:
    QXmlQuery* mXmlQuery;
    QNetworkAccessManager* mNetworkManager;
    int mCurrentIndex;
    LocationDetails* mCurrentLocationDetails;
    int mCount;
};

#endif // LocationEngine_H
