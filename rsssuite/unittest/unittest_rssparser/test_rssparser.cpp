#include "test_rssparser.h"
#include <QFile>
#include <QBuffer>
#include <QDebug>
#include <QtTest/QtTest>

test_rssparser::test_rssparser()
{
    mRSSParser = new RSSParser(this);
    mBuffer = new QBuffer(this);
}

void test_rssparser::endtoendtest()
{
}

void test_rssparser::test_setSource()
{
    QFile sourceFile(":/feed/testfeed.htm");
    if(sourceFile.open(QIODevice::ReadOnly))
    {
        // Read content to buffer
        mBuffer->setData(sourceFile.readAll());

        // Open buffer in read mode
        qDebug()<<"opening buffer "<<mBuffer->open(QIODevice::ReadOnly);

        // Set source to rssparser
        mRSSParser->setSource(mBuffer);
    }
    else
    {
    qDebug()<<"failed to open source feed file";
    }
    sourceFile.close();
}

void test_rssparser::test_channelElement_title()
{
    QString result;
    QBENCHMARK
    {
        result = mRSSParser->channelElement(RSSParser::title);
    }
    qDebug()<<result;
}

void test_rssparser::test_channelElement_link()
{
    QString result;
    QBENCHMARK
    {
        result = mRSSParser->channelElement(RSSParser::link);
    }
    qDebug()<<result;
}

void test_rssparser::test_channelElement_desc()
{
    QString result;
    QBENCHMARK
    {
        result = mRSSParser->channelElement(RSSParser::description);
    }
    qDebug()<<result;
}

// eof
