#ifndef RSSPARSER_H
#define RSSPARSER_H

#include <QObject>
#include <QXmlQuery>

struct RSSChannel
{
    QString title;
    QString link;
    QString description;
    QString imageUrl;
};

class QXmlQuery;
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
        comments,
        guid
    };

signals:

public slots:
    void setSource(QIODevice* xmlSource);
    QUrl imageUrl();
    QString channelElement(RSSElement aElement);
    QString itemElement(int itemIndex,RSSElement aElement);
    int itemCount();

private slots:
    QString executeQuery(const QString& aQuery);
    inline QString enumToString(RSSElement aElement);
private:
    QXmlQuery m_xmlQuery;
    QIODevice* m_xmlSource;
    bool m_updateRequired;
};

#endif // RSSPARSER_H
