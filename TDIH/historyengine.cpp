
#include <QDebug>
#include <QDir>
#include <QFile>
#include "historyengine.h"
#include "rssmanager.h"
#include "rssparser.h"
const QUrl KTDIHUrl("http://www.history.com/this-day-in-history/rss");
const QString KFavoritesFolder("favorites");

HistoryEngine::HistoryEngine(QObject *parent) :
               QObject(parent),
               mRSSManager(NULL),
               mInvalidateFavList(true)
{
    mRSSManager = new RSSManager(this);
    connect(mRSSManager,SIGNAL(updateAvailable(QUrl,int)),
    this,SLOT(handleUpdateAvailable(QUrl,int)));
    connect(mRSSManager,SIGNAL(error(QString,QUrl)),this,SIGNAL(error(QString,QUrl)));
    // add subscription
    mRSSManager->addFeed(FeedSubscription(KTDIHUrl));
}

void HistoryEngine::handleUpdateAvailable(QUrl sourceUrl, int updateditems)
{
    // Retrieve item
    RSSParser* parser = mRSSManager->parser(sourceUrl); // ownership is transfered
    mHistoryInfo.title = RSSParser::decodeHtml(parser->itemElement(1,RSSParser::title)).trimmed();
    mHistoryInfo.description = RSSParser::decodeHtml(parser->itemElement(1,RSSParser::description)).trimmed();
    mHistoryInfo.link = parser->itemElement(1,RSSParser::link);
    mHistoryInfo.eventDate = parser->itemElement(1,"eventDate").trimmed();

    // delete parser
    parser->deleteLater();

//    qDebug()<<mHistoryInfo.title<<"\n"<<
//              mHistoryInfo.description<<"\n"<<
//              mHistoryInfo.link<<"\n"<<
//              mHistoryInfo.eventDate;

    emit updateReady(mHistoryInfo);
}

void HistoryEngine::update()
{
qDebug()<<__PRETTY_FUNCTION__;
    if(mRSSManager)
    {
#ifdef SCREENSAVER
        mRSSManager->start(KTDIHUrl);
#else
        mRSSManager->update(KTDIHUrl);
#endif
    }
}

QStringList HistoryEngine::favoriteTitles()
{
    static QStringList favTitles;
    if(favTitles.isEmpty() || mInvalidateFavList) {
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
    return favTitles;
}

bool HistoryEngine::isFavorite(const HistoryInfo& info)
{
    QString fileToCheck = fileName(info);
    return favFileList().contains(fileToCheck);
}

bool HistoryEngine::saveAsFavorite(HistoryInfo info)
{
    bool error = false;
    QDir d;
    if(!d.exists(KFavoritesFolder)) {
      error = !(d.mkdir(KFavoritesFolder));
    }

    if(!error) {
        // TODO: check if file exists.
        error = !(QFile::copy(mRSSManager->feedFileName(KTDIHUrl),KFavoritesFolder+"/"+fileName(info)));
    }
    if(!error) {
        mInvalidateFavList = true;
    }
return error;
}

QString HistoryEngine::fileName(const HistoryInfo& info)
{
    return (QString().setNum(qHash(info.title))+".xml");
}

QStringList HistoryEngine::favFileList()
{
    static QStringList favList;
    if(mInvalidateFavList && favList.isEmpty()) {
        QDir d;
        d.setPath(KFavoritesFolder);
        if(d.exists()) {
           favList = d.entryList(QStringList(),QDir::Files,QDir::Time);
        }
    }
    mInvalidateFavList = favList.isEmpty();
    return favList;

}

// eof
