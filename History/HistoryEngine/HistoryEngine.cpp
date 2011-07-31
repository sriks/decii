/*
  Author: Srikanth Sombhatla
  Developed for: Dreamcode
  July 2011
  srikanthsombhatla@gmail.com
*/

/*!
  \class HistoryEngine
  \brief Engine to manage history content.
**/
//#define APPEND_TEST_TITLES

#include <QDebug>
#include <QDir>
#include <QFile>
#include "historyengine.h"
#include "rssmanager.h"
#include "rssparser.h"
const QUrl KTDIHUrl("http://www.history.com/this-day-in-history/rss");
const QString KFavoritesFolder("favorites");

struct HistoryEnginePrivate {
    RSSManager* rssManager;
    HistoryInfo* historyInfo;
    bool invalidateFavList;
};


HistoryEngine::HistoryEngine(QObject *parent) :
               QObject(parent)
{
    d = new HistoryEnginePrivate;
    d->invalidateFavList = true;
    d->rssManager = new RSSManager(this);
    connect(d->rssManager,SIGNAL(updateAvailable(QUrl,int)),
    this,SLOT(handleUpdateAvailable(QUrl,int)));
    connect(d->rssManager,SIGNAL(error(QString,QUrl)),this,SIGNAL(error(QString,QUrl)));
}

HistoryEngine::~HistoryEngine() {
    delete d;
}

void HistoryEngine::start() {
    // add subscription
    d->rssManager->addFeed(FeedSubscription(KTDIHUrl));
    d->rssManager->startAll();
}

QVariant HistoryEngine::historyInfo() const {
    return qVariantFromValue( (QObject*)d->historyInfo);
}

void HistoryEngine::handleUpdateAvailable(QUrl sourceUrl, int updateditems) {
    qDebug()<<__PRETTY_FUNCTION__;
    // Retrieve item
    RSSParser* parser = d->rssManager->parser(sourceUrl); // ownership is transfered
    d->historyInfo = parseInfo(parser);
    parser->deleteLater();
    emit updateReady(qVariantFromValue( (QObject*)d->historyInfo));
}

void HistoryEngine::update() {
    if(d->rssManager) {
#ifdef SCREENSAVER
        d->rssManager->start(KTDIHUrl);
#else
        d->rssManager->update(KTDIHUrl);
#endif
    }
}

QStringList HistoryEngine::favoritesList() {
    static QStringList favTitles;
    if(favTitles.isEmpty() || d->invalidateFavList) {
       QStringList favFiles = favFileList();
       RSSParser* parser = new RSSParser(this);
       for(int i=0;i<favFiles.count();++i) {
           QFile f(KFavoritesFolder+"/"+favFiles.at(i));
           if(f.open(QIODevice::ReadOnly)) {
               parser->setSource(&f);
               favTitles.append(RSSParser::decodeHtml(parser->itemElement(1,RSSParser::title)).trimmed());
           }
           f.close();
       }
       delete parser;
    }

#ifdef APPEND_TEST_TITLES
    for(int i=0;i<15;i++)
        favTitles.append("Test title");
#endif

return favTitles;
}

QVariant HistoryEngine::favorites() {
    return qVariantFromValue(favoritesList());
}

//bool HistoryEngine::isFavorite(const HistoryInfo& info)
//{
//    QString fileToCheck = fileNameForKey(info.title);
//    return favFileList().contains(fileToCheck);
//}

bool HistoryEngine::saveAsFavorite() {
    // TODO: error handling is really confusing, fix it.
    bool error = false;
    QVariant v = historyInfo();
    HistoryInfo* info = qobject_cast<HistoryInfo*>(v.value<QObject*>());
    QDir dir;
    if(!dir.exists(KFavoritesFolder))
      error = !(dir.mkdir(KFavoritesFolder));

    if(!error) {
        // TODO: check if file exists.
        // copy existing file into fav folder with required name.
        QString target = KFavoritesFolder+"/"+fileNameForKey(info->title());
        if(!QFile::exists(target))
            error = !QFile::copy(d->rssManager->feedFileName(KTDIHUrl),target);
    }

    if(!error)
        d->invalidateFavList = true;

return !error;
}

QObject* HistoryEngine::favorite(int favIndex) {
    qDebug()<<__PRETTY_FUNCTION__<<" "<<favIndex;
    HistoryInfo* info;
    QString favTitle = favoritesList().at(favIndex);
    qDebug()<<__PRETTY_FUNCTION__<<" title:"<<favTitle;
        QFile f(KFavoritesFolder+"/"+fileNameForKey(favTitle));
        if(f.open(QIODevice::ReadOnly)) {
            RSSParser* parser = new RSSParser(this);
            parser->setSource(&f);
            info = parseInfo(parser);
        }
        else
            qWarning()<<__PRETTY_FUNCTION__<<" unable to open "<<f.fileName();
return info;
}

QString HistoryEngine::fileNameForKey(const QString& key) {
    return (QString().setNum(qHash(key))+".xml");
}

QStringList HistoryEngine::favFileList() {
    static QStringList favList;
    if(/*d->invalidateFavList && */favList.isEmpty()) { // TODO: fix cache invalidation
        QDir d;
        d.setPath(KFavoritesFolder);
        if(d.exists()) {
           favList = d.entryList(QStringList(),QDir::Files,QDir::Time);
        }
    }
    favList.removeDuplicates();
    d->invalidateFavList = favList.isEmpty();
    return favList;
}

HistoryInfo* HistoryEngine::parseInfo(RSSParser* parser) {
    HistoryInfo* info = new HistoryInfo(this);
    info->mTitle = RSSParser::decodeHtml(parser->itemElement(1,RSSParser::title)).trimmed();
    info->mDescription = RSSParser::decodeHtml(parser->itemElement(1,RSSParser::description)).trimmed();
    info->mLink = parser->itemElement(1,RSSParser::link);
    info->mEventDate = parser->itemElement(1,"eventDate").trimmed();
    return info;
}

int HistoryEngine::favoritesCount() {
    return favoritesList().count();
}

// eof
