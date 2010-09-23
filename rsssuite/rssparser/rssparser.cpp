#include <QIODevice>
#include <QUrl>
#include <QMetaEnum>
#include <QMetaObject>
#include <QDebug>
#include <QXmlItem>
#include "rssparser.h"

// FIXME: handle encoding properly
// XQueries
//
// Get channel image url
const QString KXqChannelImageUrl("let $image := doc($xmlSource)//channel/image return (data($image/*:url))");
// Query an element in channel other than image
const QString KXqChannelElementQuery("let $channel := doc($xmlSource)//channel return (data($channel/*:%1))");
// Query Item count
const QString KXqItemCount("let $x := doc($xmlSource)//item return count($x)");
// Query element of an item identified with index
const QString KXqItemQuery("let $item := doc($xmlSource)//item[%1] return (data($item/*:%2))");
// Query all item elements
// fn:string is required by QXmlQuery to evaluate to QSrtringList
// fn:string requires index, so running a for loop.
// TODO: can this query be optimized?
const QString KXqAllItemElementsQuery("for $root in doc($xmlSource)//channel/item return string($root/*:%1)");

// Query categories in an item
const QString KXqItemCategories("let $category := doc($xmlSource)//channel/item[%1]/*:category for $categorylist in $category return string($categorylist)");
const QString KXmlSource("xmlSource");


/**
  RSSParser is a special xml parser for RSS feeds. It is based on XQuery as backend. It uses QIODevice to look at the xml source.
  Hence it is very light weight as it does not own the data source. Using RSSParser requires some basic knowledge of RSS elements.

  \code

    QRSSParser parser;
    QFile sourceFile("rss.xml"); // file in which rss feed is collected/stored.
    if(source.open(QIODevice::ReadOnly))
    {
       // set source
       parser.setSource(sourceFile);

       if(parser.isValid())
       {
       // get name of feed
       QString channelname = parser.channelElement(RSSParser::title);

       // get count of items
       int count = parser.itemCount();

       // get title of first item in feed
       QString title = parser.itemElement(1,RSSParser::title); // XQuery numbering starts from 1
       }
    }
    sourceFile.close();

  \endcode

  Parsing is done on two different elements namely Channel and Item. Evert RSS feed has one and only one channel in which there
  can be any or no items. All possible RSS Elements are provided in \enum RSSElement enum. Two methods namely \fn channelElement() and
  \fn itemElement() are used to retieve channel and item elements respectively.

  Some special methods are provided seperately for querying some channel and item elements like \fn itemCount()
  **/



RSSParser::RSSParser(QObject *parent) :
    QObject(parent)
{
    m_xmlSource = NULL;
    m_IsError = false;
}

RSSParser::~RSSParser()
{
    qDebug()<<__FUNCTION__;
}

/**
  \brief Validates parser.
  Parser is valid if a valid and readable QIODevice is set as source.
  \return true if valid
  **/
bool RSSParser::isValid()
{
    if(m_xmlSource)
    {
    return ( (m_xmlSource->isReadable()) &&
             (m_xmlSource->size()) );
    }

    else
    {
    return !m_xmlSourceFileName.isEmpty();
    }
}

void RSSParser::setSource(QIODevice* xmlSource)
{
      m_xmlSource = xmlSource;
      m_xmlQuery.bindVariable(KXmlSource,xmlSource);
}

bool RSSParser::setSourceFileName(QString sourceFileName)
{
    if(sourceFileName.isEmpty())
    {return false;}
    m_xmlSourceFileName = sourceFileName;
    m_xmlQuery.bindVariable(KXmlSource,QXmlItem(QVariant(m_xmlSourceFileName)));
    m_xmlSource = NULL;
    return true;
}

QString RSSParser::executeQuery(const QString& aQuery)
{
    m_IsError = false;
    m_xmlQuery.setQuery(aQuery);
    QString result = QString();

    if(m_xmlQuery.isValid())
    {
       m_xmlQuery.evaluateTo(&result);
    }

    else
    {
        m_IsError = true;
        qWarning()<<__FUNCTION__<<" invalid query \n"<<aQuery;
    }
    return result;
}

QStringList RSSParser::executeQueryAsList(const QString& aQuery)
{
    m_IsError = false;
    m_xmlQuery.setQuery(aQuery);
    QStringList result;
    result.clear();

    if(m_xmlQuery.isValid())
    {
       m_xmlQuery.evaluateTo(&result);
    }

    else
    {
        m_IsError = true;
        qWarning()<<__FUNCTION__<<" invalid query \n"<<aQuery;
    }
    return result;
}

QUrl RSSParser::imageUrl()
{
    return QUrl(executeQuery(KXqChannelImageUrl));
}

QString RSSParser::channelElement(RSSElement aElement)
{
    QString enumString = enumToString(aElement);
    return executeQuery(KXqChannelElementQuery.arg(enumString));
}

QString RSSParser::itemElement(int itemIndex,RSSElement aElement)
{
    QString enumString = enumToString(aElement);
    return executeQuery(KXqItemQuery.arg(itemIndex).arg(enumString));
}

QStringList RSSParser::itemElements(RSSElement aElement)
{
    QString enumString = enumToString(aElement);
    return executeQueryAsList(KXqAllItemElementsQuery.arg(enumString));
}

QStringList RSSParser::category(int itemIndex)
{
    // Query all items
    if(itemIndex)
    {
    return executeQueryAsList(KXqItemCategories.arg(QString().number(itemIndex)));
    }
return QStringList();
}

QList<QStringList> RSSParser::categories()
{
    QList<QStringList> list;
    int count = itemCount();
    for(int i=1;i<=count;i++) // XQuery indexing starts with 1
    {
        list<<category(i);
    }
    return list;
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
