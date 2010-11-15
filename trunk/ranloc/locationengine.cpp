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
#ifdef BUG_8687
#include "S60QHttp.h"
#endif

#include "locationengine.h"

const QByteArray KCurrentIndex("currentindex");
const QString KFileName("locations.xml");
const QString KXmlSource("xmlsource");
const QString KBaseQuery("let $entry := doc($xmlsource)//geonames/entry ");
const QString KGetEntryByIndex("return $entry[%1]");
const QString KEntriesCount("return count($entry)");
const QString KGetEntryElement("return data($entry[%1]/%2)");

//const QString KGetEntryByIndex("let $entry := doc(\"locations.xml\")//geonames/entry return $entry[1]");

LocationEngine::LocationEngine(QObject* parent)
               :QObject(parent),
                mCurrentLocationDetails(NULL),
                mCount(0)
{
#ifdef BUG_8687
    mHttp = new S60QHttp(this);

#else
    mNetworkManager = new QNetworkAccessManager(this);
    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
#endif
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
    mCurrentIndex++;
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
    mCurrentLocationDetails->link = readElement(&file,mCurrentIndex,LocationEngine::wikipediaUrl);
    mCurrentLocationDetails->imageLink = readElement(&file,mCurrentIndex,LocationEngine::thumbnailImg);
    }
    file.close();
    return mCurrentLocationDetails;
}

#ifdef BUG_8687
void LocationEngine::on_http_requestFinished (int id,bool error)
{
    if(error)
    {
        emit errorOccured(mHttp->errorString());
        return;
    }
    handleContent(mHttp->readAll());
}
#else
void LocationEngine::replyFinished(QNetworkReply *reply)
{
    if(QNetworkReply::NoError != reply->error())
    {
        emit errorOccured(reply->errorString());
        return;
    }
    handleContent(reply->readAll());
}
#endif

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
    qDebug()<<elementName;
    query = query.arg(elementName);
    xmlSource->reset();
    return executeQuery(xmlSource,query);
}

QString LocationEngine::executeQuery(QIODevice* xmlSource, const QString& aQuery)
{
    qDebug()<<aQuery;

    QXmlQuery xmlQuery;
    xmlQuery.bindVariable(KXmlSource,xmlSource);
    xmlQuery.setQuery(aQuery);
    QString result = QString();
    if(xmlQuery.isValid())
    {
       xmlQuery.evaluateTo(&result);
    }
    return result;
}

// eof
