#include <QtGui/QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGraphicsObject>
#include "WeatherbugInfo.h"
#include "WeatherManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("WeatherLiteHD");
    QApplication::setOrganizationName("Dreamcode");
    QCoreApplication::setAttribute(Qt::AA_S60DisablePartialScreenInputMode, false);
    QDeclarativeView dv;
    Qt::WidgetAttribute attr;
    attr = Qt::WA_LockPortraitOrientation;
    dv.setAttribute(attr);
    QDeclarativeContext* context = dv.rootContext();
    WeatherManager m;
    context->setContextProperty("weatherManager",(QObject*)&m);
    dv.setSource(QUrl("qrc:/qml/main.qml"));
    QObject *rootObject = dynamic_cast<QObject*>(dv.rootObject());
    QObject::connect(&m,SIGNAL(showInfoBanner(QVariant)),rootObject,SLOT(showBanner(QVariant)));
    dv.showFullScreen();
    return app.exec();
}
