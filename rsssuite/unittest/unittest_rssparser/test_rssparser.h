#ifndef TEST_RSSPARSER_H
#define TEST_RSSPARSER_H

#include <QObject>
#include "rssparser.h"

class QBuffer;
class test_rssparser : public QObject
{
    Q_OBJECT

public:
    test_rssparser();
    void endtoendtest();

private slots:
    //void test_endtoend(){endtoendtest();}
    void test_setSource();
    void test_channelElement_title();
    void test_channelElement_link();
    void test_channelElement_desc();
private:
    RSSParser* mRSSParser;
    QBuffer* mBuffer;
};

#endif // TEST_RSSPARSER_H
