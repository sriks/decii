#ifndef HISTORYENGINE_H
#define HISTORYENGINE_H
#include <QObject>
#include <QUrl>

class HistoryInfo:public QObject
{
    Q_OBJECT
public:
    HistoryInfo(QObject* parent):
                QObject(parent){}
    Q_INVOKABLE QString title() {
        return mTitle;
    }
    Q_INVOKABLE QString description() {
        return mDescription;
    }
    Q_INVOKABLE QUrl link() {
        return mLink;
    }
    QString eventDate() {
        // TODO: convert to correct date format with QDate
        return mEventDate;
    }
    QString mTitle;
    QString mDescription;
    QUrl mLink;
    QString mEventDate;
};

class RSSManager;
class RSSParser;
//class HistoryInfo;
class HistoryEnginePrivate;
class HistoryEngine : public QObject
{
    Q_OBJECT
public:
    explicit HistoryEngine(QObject *parent = 0);
    ~HistoryEngine();
    Q_INVOKABLE HistoryInfo* historyInfo() const;
//    QStringList favoriteTitles();
//    HistoryInfo favorite(QString favTitle);
//    bool isFavorite(const HistoryInfo& info);
//    bool saveAsFavorite(HistoryInfo info);

signals:
    void updateReady(HistoryInfo* info);
    void error(QString errorDescription,QUrl sourceUrl);

public slots:
    void update();

private slots:
    void handleUpdateAvailable(QUrl sourceUrl, int updateditems);

private:
    HistoryInfo* parseInfo(RSSParser* parser);

private:
    QString fileNameForKey(const QString& key);
    QStringList favFileList();

private:
    HistoryEnginePrivate* d;
};

#endif // HISTORYENGINE_H
