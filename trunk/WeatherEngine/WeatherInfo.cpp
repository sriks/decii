#include <QTime>
#include "WeatherInfo.h"
#include "Constants.h"

struct WeatherInfoPrivate {
    QTime lastUpdatedTime;
};

WeatherInfo::WeatherInfo(QObject *parent) :
    WeatherForecastInfo(parent) {
    d = new WeatherInfoPrivate;

    connect(this,SIGNAL(updated(WeatherInfo*)),this,SLOT(updateLastUpdatedTime()),Qt::UniqueConnection);
    connect(this,SIGNAL(updated(WeatherInfo*)),this,SIGNAL(temperatureChanged()),Qt::UniqueConnection);
    connect(this,SIGNAL(updated(WeatherInfo*)),this,SIGNAL(conditionChanged()),Qt::UniqueConnection);
    connect(this,SIGNAL(updated(WeatherInfo*)),this,SIGNAL(windDirectionChanged()),Qt::UniqueConnection);
    connect(this,SIGNAL(updated(WeatherInfo*)),this,SIGNAL(windSpeedChanged()),Qt::UniqueConnection);
    connect(this,SIGNAL(updated(WeatherInfo*)),this,SIGNAL(moonPhaseChanged()),Qt::UniqueConnection);
    connect(this,SIGNAL(updated(WeatherInfo*)),this,SIGNAL(ampmChanged()),Qt::UniqueConnection);
}

WeatherInfo::~WeatherInfo() {
    delete d;
}

QString WeatherInfo::locationId() const {
    QString();
}

QString WeatherInfo::title() const {
    return this->weatherData().value(PROP_CITY,QString()).toString();
}

QString WeatherInfo::subTitle() const {
    return this->weatherData().value(PROP_COUNTRY,QString()).toString();
}

QObject * WeatherInfo::forecast(int numOfdays) {
    return NULL;
}

QString WeatherInfo::serviceProviderName() const {
    return this->weatherData().value(PROP_SERVICEPROVIDERNAME,QString()).toString();
}

QString WeatherInfo::serviceProviderUrl() const {
    return QString();
}

QString WeatherInfo::lastUpdatedTime() const {
    return d->lastUpdatedTime.toString();
}

void WeatherInfo::setInfo(QObject* info) {
    Q_UNUSED(info)
}

QObject * WeatherInfo::info() const {
    return NULL;
}

void WeatherInfo::updateLastUpdatedTime() {
    d->lastUpdatedTime = QTime::currentTime();
    emit lastUpdatedTimeChanged();
}

// eof
