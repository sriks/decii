#ifndef TEST_RSSMANAGER_H
#define TEST_RSSMANAGER_H

#include <QObject>
#include "rssmanager.h"

class test_rssmanager : public QObject
{
    Q_OBJECT
public:
    explicit test_rssmanager(QObject *parent = 0);
    void manualTestAll()
    {
    initTestCase();
    testAll();
    }

signals:

public slots:
    void handleUpdateAvailable(RSSParser* parser, int updateditems);
private slots:
    void initTestCase();
    void listAllSubscriptions();
    void testRemoveSubscriptions();
    void testAll();

private:
    RSSManager* mRSSManager;
};

#endif // TEST_RSSMANAGER_H
