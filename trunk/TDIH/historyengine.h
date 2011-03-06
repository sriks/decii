#ifndef HISTORYENGINE_H
#define HISTORYENGINE_H
#include <QObject>
#include <QUrl>

struct HistoryInfo
{
    QString title;
    QString description;
    QUrl link;
    QString eventDate;
};

class RSSManager;
class RSSParser;
class HistoryEngine : public QObject
{
    Q_OBJECT
public:
    explicit HistoryEngine(QObject *parent = 0);
    QStringList favoriteTitles();
    bool isFavorite(const HistoryInfo& info);
signals:
    void updateReady(HistoryInfo info);
    void error(QString errorDescription,QUrl sourceUrl);

public slots:
    void update();
    bool saveAsFavorite(HistoryInfo info);
private slots:
    void handleUpdateAvailable(QUrl sourceUrl, int updateditems);

private:
    QString fileName(const HistoryInfo& info);
    QStringList favFileList();

private:
    RSSManager* mRSSManager;
    HistoryInfo mHistoryInfo;
    bool mInvalidateFavList;
};

#endif // HISTORYENGINE_H
