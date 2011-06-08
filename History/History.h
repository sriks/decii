#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>

class HistoryInfo;
class HistoryPrivate;
class History : public QObject
{
    Q_OBJECT
public:
    explicit History(QObject *parent = 0);
    ~History();

    void startEngine();
signals:

public slots:
    void showQmlView();
    void onUpdateAvailable(HistoryInfo* info);
private:
    HistoryPrivate* d;
};

#endif // HISTORY_H
