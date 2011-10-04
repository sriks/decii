#ifndef XQUERYENGINE_H
#define XQUERYENGINE_H

#include <QObject>
#include <QIODevice>
#include <QVariant>
#include <QStringList>
struct XQueryEnginePrivate;
class XQueryEngine : public QObject
{
    Q_OBJECT
public:
    explicit XQueryEngine(QObject *parent = 0);
    ~XQueryEngine();
public slots:
    QString executeQuery(QString query,QIODevice* source);
    QStringList executeQueryAsList(QString query,QIODevice* source);
    bool isSuccess() const;
private:
    XQueryEnginePrivate* d;
};

#endif // XQUERYENGINE_H
