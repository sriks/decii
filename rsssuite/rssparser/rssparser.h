#ifndef RSSPARSER_H
#define RSSPARSER_H

#include <QObject>
#include <QXmlQuery>
#include <QStringList>

class QUrl;
class QIODevice;
class RSSParser : public QObject
{
    Q_OBJECT
    Q_ENUMS(RSSElement)

public:
    explicit RSSParser(QObject *parent = 0);
    ~RSSParser();
    enum RSSElement
    {
        title,
        link,
        description,
        pubDate,
        language,
        lastBuildDate,
        generator,
        copyright,
        skipHours,
        skipDays,
        // Pure Item elements
        author,
        creator, // TODO: Check diff ns like dublin
        comments,
        guid
    };

signals:

public slots:
    /**
      returns true if it is a valid parser
    **/
    Q_INVOKABLE bool isValid();

    /**
      set source for parser
    **/

    Q_INVOKABLE void setSource(QIODevice* xmlSource);
    Q_INVOKABLE bool setSourceFileName(QString sourceFileName);
    Q_INVOKABLE QUrl imageUrl();
    Q_INVOKABLE QString channelElement(RSSElement aElement);
    Q_INVOKABLE QString itemElement(int itemIndex,RSSElement aElement);
    // queries aElement from all items, like titles of all items
    Q_INVOKABLE QStringList itemElements(RSSElement aElement);
    // returns list of categories for a item identified by itemIndex
    Q_INVOKABLE QStringList category(int itemIndex);
    // convinience method to query all categories
    // returns a list of stringlist of categories for all items.
    // if an item doest have a category, stringlist in its index will be empty.
    Q_INVOKABLE QList<QStringList> categories();
    Q_INVOKABLE int itemCount();
    // TODO: add method to query if an element exists

    // Returns if the latest call resulted error
    // error is not persistent across calls
    bool isError(){return m_IsError;}

private slots:
    QString executeQuery(const QString& aQuery);
    QStringList executeQueryAsList(const QString& aQuery);
    QString enumToString(RSSParser::RSSElement aElement);

private:
    QXmlQuery m_xmlQuery;
    QIODevice* m_xmlSource;
    QString m_xmlSourceFileName;
    bool m_IsError;
};

#endif // RSSPARSER_H
