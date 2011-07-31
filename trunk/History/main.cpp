#include <QtGui/QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGraphicsObject>
#include <QObject>
#include "HistoryEngine.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    HistoryEngine engine;
    QDeclarativeView dv;
#ifdef LOCK_POTRAITORIEN
    Qt::WidgetAttribute attr;
    attr = Qt::WA_LockPortraitOrientation;
    dv.setAttribute(attr);
#endif
    QDeclarativeContext* context = dv.rootContext();
    context->setContextProperty("engine",(QObject*)&engine);
    dv.setSource(QUrl("qrc:/qml/main.qml"));
    QObject *rootObject = dynamic_cast<QObject*>(dv.rootObject());
    QObject::connect(&engine,SIGNAL(updateReady(QVariant)),rootObject,SLOT(onUpdateAvailable()));
    QObject::connect(&engine,SIGNAL(error(QString,QUrl)),rootObject,SLOT(error(QString)));
    dv.showFullScreen();



//    QmlApplicationViewer viewer;
//    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);
//    viewer.setMainQmlFile(QLatin1String("qml/main.qml"));
//    viewer.showExpanded();
//    viewer.showFullScreen();
    return app.exec();
}
