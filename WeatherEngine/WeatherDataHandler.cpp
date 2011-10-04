#include <QFile>
#include <QDebug>
#include <QBuffer>
#include <QVariant>
#include <QStringList>
#include <QStringListIterator>
#include "WeatherDataHandler.h"
#include "XQueryEngine.h"

const QString KNormalizedDataDelimiter = "|!";

struct WeatherDataHandlerPrivate {
    QByteArray normalizedWeatherXml;
    QByteArray normalizedForecastXml;
    QVariantMap props;
    QStringList weatherData;
    XQueryEngine* xquery;

    WeatherDataHandlerPrivate() {
        xquery = new XQueryEngine;
    }

    ~WeatherDataHandlerPrivate() {
        xquery->deleteLater();
    }
};

WeatherDataHandler::WeatherDataHandler(QObject *parent)
                   :QObject(parent) {
    d = new WeatherDataHandlerPrivate;
}

WeatherDataHandler::~WeatherDataHandler() {
    delete d;
}

void WeatherDataHandler::handleNormalizedWeatherXml(QString normalizedXml) {
    if(!normalizedXml.isEmpty()) {
        d->normalizedWeatherXml = normalizedXml.toUtf8();
        d->normalizedWeatherXml = d->normalizedWeatherXml.trimmed();
        parseWeatherData(d->normalizedWeatherXml);
    }
}

void WeatherDataHandler::parseWeatherData(QByteArray data) {
    QBuffer b(&data);
    QFile f(":/resources/xq/weatherdatafromnormalizedxml.xq");
    if(f.open(QIODevice::ReadOnly) && b.open(QIODevice::ReadOnly)) {
        QString q = f.readAll();
        QVariant result = d->xquery->executeQueryAsList(q,&b);
        if(result.isValid() && d->xquery->isSuccess()) {
            QStringList resultList = result.toStringList();
            QStringListIterator iter(resultList);
            while(iter.hasNext()) {
                QStringList data = iter.next().split(KNormalizedDataDelimiter);
                if(data.size())
                    setWeatherData(data.at(0),data.at(1));
            }
        }
    }
}

QVariantMap WeatherDataHandler::weatherData() const {
    return d->props;
}

bool WeatherDataHandler::isSupports(QString featureName) const {
    return d->props.contains(featureName.toLower());
}

void WeatherDataHandler::setWeatherData(QString featureName, QVariant value) {
    d->props.insert(featureName,value);
}

void WeatherDataHandler::printWeatherData() const {
    qDebug()<<Q_FUNC_INFO<<d->props;
}

// eof
