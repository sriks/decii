#include <QXmlQuery>
#include <QStringList>
#include "XQueryEngine.h"

const QString KSrc = "src";
struct XQueryEnginePrivate {
    bool success;
    QXmlQuery* query;
    XQueryEnginePrivate() {
        query = new QXmlQuery;
        success = true;
    }
    ~XQueryEnginePrivate() {
        delete query;
    }
};

XQueryEngine::XQueryEngine(QObject *parent) :
    QObject(parent) {
    d = new XQueryEnginePrivate;
}

XQueryEngine::~XQueryEngine() {
    delete d;
}

QString XQueryEngine::executeQuery(QString query, QIODevice* source) {
    d->query->bindVariable(KSrc,source);
    d->query->setQuery(query);
    QString result = QString();
    d->success = false;
    if(d->query->isValid())
       d->success = d->query->evaluateTo(&result);
    return result;
}

QStringList XQueryEngine::executeQueryAsList(QString query, QIODevice *source) {
    d->query->bindVariable(KSrc,source);
    d->query->setQuery(query);
    QStringList result;
    d->success = false;
    if(d->query->isValid())
       d->success = d->query->evaluateTo(&result);
    return result;
}

bool XQueryEngine::isSuccess() const {
    return d->success;
}

// eof
