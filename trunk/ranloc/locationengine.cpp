#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QSettings>
#include <QXmlQuery>
#include <QIODevice>
#include <QMetaEnum>
#include <QFile>
#include "locationengine.h"

#ifdef BUG_8687
#include <es_sock.h>
#include <sys/socket.h>
#include <net/if.h>

static void setDefaultIapL()
    {
    RSocketServ serv;
    CleanupClosePushL(serv);
    User::LeaveIfError(serv.Connect());

    RConnection conn;
    CleanupClosePushL(conn);
    User::LeaveIfError(conn.Open(serv));
    User::LeaveIfError(conn.Start());

    _LIT(KIapNameSetting, "IAP\\Name");
    TBuf8<50> iap8Name;

    User::LeaveIfError(conn.GetDesSetting(TPtrC(KIapNameSetting), iap8Name));
    iap8Name.ZeroTerminate();

    conn.Stop();
    CleanupStack::PopAndDestroy(&conn);
    CleanupStack::PopAndDestroy(&serv);

    struct ifreq ifReq;
    memset(&ifReq, 0, sizeof(struct ifreq));
    strcpy( ifReq.ifr_name, (char*)iap8Name.Ptr());
    User::LeaveIfError(setdefaultif( &ifReq ));
}

static int setDefaultIap()
{
    TRAPD(err, setDefaultIapL());
    qDebug()<<"Error in setDefaultIap: "<<err;
    return err;
}
#endif

const QByteArray KCurrentIndex("currentindex");
const QString KFileName("locations.xml");
const QString KXmlSource("xmlsource");
const QString KBaseQuery("let $entry := doc($xmlsource)//geonames/entry ");
const QString KGetEntryByIndex("return $entry[%1]");
const QString KEntriesCount("return count($entry)");
const QString KGetEntryElement("return data($entry[%1]/%2)");

const QString KXqCountryInfoForCountryCode("countryinfoforcountrycode.xq");

LocationEngine::LocationEngine(QObject* parent)
               :QObject(parent),
                mCurrentLocationDetails(NULL),
                mCount(0)
{
#ifdef BUG_8687
    setDefaultIap();
#endif
    mXmlQuery = new QXmlQuery;
    mNetworkManager = new QNetworkAccessManager(this);
    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));


    QSettings settings(QApplication::organizationName(),QApplication::applicationName());
    if(settings.contains(KCurrentIndex))
    {
        mCurrentIndex = settings.value(KCurrentIndex).toInt();
    }
    else
    {
        mCurrentIndex = 1;
    }
}

LocationEngine::~LocationEngine()
{
    delete mXmlQuery;
    QSettings settings(QApplication::organizationName(),QApplication::applicationName());
    settings.setValue(KCurrentIndex,mCurrentIndex);
}

void LocationEngine::newLocation(QUrl url)
{
    mCurrentIndex = 0;
    mNetworkManager->get(QNetworkRequest(url));
}

LocationDetails* LocationEngine::nextLocation()
{
    ++mCurrentIndex;
    return LocationDetails(mCurrentIndex);
 }

LocationDetails* LocationEngine::prevLocation()
{
    --mCurrentIndex;
    return LocationDetails(mCurrentIndex);
}

LocationDetails* LocationEngine::locationDetails(int index)
{
    delete mCurrentLocationDetails;
    mCurrentLocationDetails = new LocationDetails(this);
    QFile file(KFileName);
    if(file.open(QIODevice::ReadOnly))
    {
    mCurrentLocationDetails->title = readElement(&file,mCurrentIndex,LocationEngine::title);
    mCurrentLocationDetails->summary = readElement(&file,mCurrentIndex,LocationEngine::summary);
    mCurrentLocationDetails->countryCode = readElement(&file,mCurrentIndex,LocationEngine::countryCode);
    mCurrentLocationDetails->latitude = readElement(&file,mCurrentIndex,LocationEngine::lat);
    mCurrentLocationDetails->longitude = readElement(&file,mCurrentIndex,LocationEngine::lng);
    mCurrentLocationDetails->link = readElement(&file,mCurrentIndex,LocationEngine::wikipediaUrl).trimmed();
    mCurrentLocationDetails->imageLink = readElement(&file,mCurrentIndex,LocationEngine::thumbnailImg).trimmed();
    }
    file.close();
    return mCurrentLocationDetails;
}

void LocationEngine::replyFinished(QNetworkReply *reply)
{
    if(QNetworkReply::NoError != reply->error())
    {
        emit errorOccured(reply->errorString());
        return;
    }
    handleContent(reply->readAll());
}

void LocationEngine::handleContent(QByteArray content)
{
    qDebug()<<content;
    QFile::remove(KFileName);
    QFile file(KFileName);
    file.open(QIODevice::ReadWrite);
    QString tmp(content);
    file.write(tmp.toUtf8());
    QString query(KBaseQuery + KEntriesCount);
    file.reset();
    mCount = executeQuery(&file,query).toInt();
    file.close();
    emit newLocationAvailable();
}

QString LocationEngine::readElement(QIODevice* xmlSource,int index,EntryElement element)
{
    QString query(KBaseQuery + KGetEntryElement);
    query = query.arg(index);
    int enumIndex = metaObject()->indexOfEnumerator("EntryElement");
    QMetaEnum menum = metaObject()->enumerator(enumIndex);
    QString elementName = menum.valueToKey(element);
    query = query.arg(elementName);
    xmlSource->reset();
    return executeQuery(xmlSource,query);
}

QString LocationEngine::executeQuery(QIODevice* xmlSource, const QString& aQuery)
{
//    qDebug()<<aQuery;
    mXmlQuery->bindVariable(KXmlSource,xmlSource);
    mXmlQuery->setQuery(aQuery);
    QString result = QString();
    if(mXmlQuery->isValid())
    {
       mXmlQuery->evaluateTo(&result);
    }
    return result;
}

// eof
