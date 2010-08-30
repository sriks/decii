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
    enum RSSElement
    {
        title,
        link,
        description,
        pubDate,
        language,
        lastBuildDate,
        category,
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
    Q_INVOKABLE void setSource(QIODevice* xmlSource);
    Q_INVOKABLE QUrl imageUrl();
    Q_INVOKABLE QString channelElement(RSSElement aElement);
    Q_INVOKABLE QString itemElement(int itemIndex,RSSElement aElement);
    Q_INVOKABLE QStringList itemElements(RSSElement aElement);
    Q_INVOKABLE int itemCount();

    // TODO: add method to query if an element exists
private slots:
    QString executeQuery(const QString& aQuery);
    QStringList executeQueryAsList(const QString& aQuery);
    QString enumToString(RSSParser::RSSElement aElement);

private:
    QXmlQuery m_xmlQuery;
    QIODevice* m_xmlSource;
};

#endif // RSSPARSER_H
