#include "History.h"
#include "qmlapplicationviewer.h"
#include "HistoryEngine.h"
struct HistoryPrivate {
    HistoryEngine* engine;
    QmlApplicationViewer* qmlViewer;
    ~HistoryPrivate() {
        delete qmlViewer;
        qmlViewer = NULL;
    }
};

History::History(QObject *parent) :
    QObject(parent) {
    d = new HistoryPrivate;
    d->engine = new HistoryEngine(this);
    connect(d->engine,SIGNAL(updateReady(HistoryInfo*)),this,SLOT(onUpdateAvailable(HistoryInfo*)));
    d->qmlViewer = new QmlApplicationViewer;
}

History::~History() {
    delete d;
}

void History::startEngine() {
    d->engine->update();
}

void History::onUpdateAvailable(HistoryInfo *info) {
    // notify qml view
}

