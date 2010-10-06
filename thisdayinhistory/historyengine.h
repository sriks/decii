#ifndef HISTORYENGINE_H
#define HISTORYENGINE_H
#include <QObject>
#include <QUrl>

struct HistoryInfo
{
    QString title;
    QString description;
    QUrl link;
};

class RSSManager;
class RSSParser;
class HistoryEngine : public QObject
{
    Q_OBJECT
public:
    explicit HistoryEngine(QObject *parent = 0);

signals:
    void updateReady(HistoryInfo info);
public slots:
    void update();
private slots:
    void handleUpdateAvailable(QUrl sourceUrl, int updateditems);

private:
    RSSManager* mRSSManager;
    HistoryInfo mHistoryInfo;
};

#endif // HISTORYENGINE_H
