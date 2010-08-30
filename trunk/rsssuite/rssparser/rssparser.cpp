
#include <QIODevice>
#include <QUrl>
#include <QMetaEnum>
#include <QMetaObject>
#include <QDebug>
#include "rssparser.h"

// FIXME: handle encoding properly
// XQueries
//
// Get channel image url
const QString KXqChannelImageUrl("let $image := doc($xmlSource)//channel/image return (data($image/url))");
// Query an element in channel other than image
const QString KXqChannelElementQuery("let $channel := doc($xmlSource)//channel return (data($channel/%1))");
// Query Item count
const QString KXqItemCount("let $x := doc($xmlSource)//item return count($x)");
// Query element of an item identified with index
const QString KXqItemQuery("let $item := doc($xmlSource)//item[%1] return (data($item/%2))");

// FIXME: string requires index, comeup with a general query that doest need index
// Query all item elements
const QString KXqAllItemElementsQuery("let $root := doc($xmlSource) return xs:string($root//channel/item[1]/%1)");

RSSParser::RSSParser(QObject *parent) :
    QObject(parent)
{
}

void RSSParser::setSource(QIODevice* xmlSource)
{
      m_xmlSource = xmlSource;
      m_xmlQuery.bindVariable("xmlSource",xmlSource);
}

QString RSSParser::executeQuery(const QString& aQuery)
{
    qDebug()<<__FUNCTION__;
    qDebug()<<aQuery;
    m_xmlQuery.setQuery(aQuery);
    QString result = QString();

    if(m_xmlQuery.isValid())
    {
       qDebug()<<m_xmlQuery.evaluateTo(&result);
    }

    else
    {
        qDebug()<<__FUNCTION__<<" invalid query";
    }
    return result;
}

QStringList RSSParser::executeQueryAsList(const QString& aQuery)
{
    qDebug()<<__FUNCTION__;
    qDebug()<<aQuery;

    m_xmlQuery.setQuery(aQuery);
    QStringList result;
    result.clear();

    if(m_xmlQuery.isValid())
    {
       qDebug()<<m_xmlQuery.evaluateTo(&result);
    }

    else
    {
        qDebug()<<__FUNCTION__<<" invalid query";
    }
    return result;
}

QUrl RSSParser::imageUrl()
{
    qDebug()<<__FUNCTION__;
    return QUrl(executeQuery(KXqChannelImageUrl));
}

QString RSSParser::channelElement(RSSElement aElement)
{
    qDebug()<<__FUNCTION__;
    QString enumString = enumToString(aElement);
    return executeQuery(KXqChannelElementQuery.arg(enumString));
}

QString RSSParser::itemElement(int itemIndex,RSSElement aElement)
{
    qDebug()<<__FUNCTION__;
    QString enumString = enumToString(aElement);
    return executeQuery(KXqItemQuery.arg(itemIndex).arg(enumString));
}

QStringList RSSParser::itemElements(RSSElement aElement)
{
    QString enumString = enumToString(aElement);
    return executeQueryAsList(KXqAllItemElementsQuery.arg(enumString));
}

int RSSParser::itemCount()
{
    return executeQuery(KXqItemCount).toInt();
}

QString RSSParser::enumToString(RSSParser::RSSElement aElement)
{
    int index = metaObject()->indexOfEnumerator("RSSElement");
    QMetaEnum menum = metaObject()->enumerator(index);
    return menum.valueToKey(aElement);
}

// end of file