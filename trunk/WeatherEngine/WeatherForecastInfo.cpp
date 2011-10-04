#include <QDebug>
#include <QDateTime>
#include "WeatherForecastInfo.h"
#include "Constants.h"
struct WeatherForecastInfoPrivate {};

WeatherForecastInfo::WeatherForecastInfo(QObject *parent) :
    WeatherDataHandler(parent) {
    d = new WeatherForecastInfoPrivate;

}

WeatherForecastInfo::~WeatherForecastInfo() {
    delete d;
}

QString WeatherForecastInfo::temperature() {
    return weatherData().value(PROP_TEMPERATURE).toString();
}

QString WeatherForecastInfo::temperatureUnits() {
    return weatherData().value(PROP_TEMPERATUREUNITS).toString();
}

QString WeatherForecastInfo::condition() {
    return weatherData().value(PROP_CONDITION,QString()).toString();
}

QString WeatherForecastInfo::conditionImages() {
    return weatherData().value(PROP_CONDITIONIMAGE,QString()).toString();
}

QString WeatherForecastInfo::moonPhase() const {
    return weatherData().value(PROP_MOONPHASE,QString()).toString();
}

QString WeatherForecastInfo::ampm() const {
    /*
    Formula for long:
    long = (timeAtlocation - utcTime)*15
    Hence timeAtLocation = (long/15)+utcTime
    The result is approx time, which should be enough to roughly calculate whether it is sun or moon at sky.
    TODO: Name of the method is misleading. Change it.
    */

    QString result;
    QDateTime currentUtc = QDateTime::currentDateTimeUtc();
    float utcHours = currentUtc.time().hour();
    float lon = longitude().toFloat();
    float timeAtLocation = (lon/15)+utcHours;
    if(timeAtLocation<0)
        timeAtLocation *= -1;
    if(timeAtLocation > 24)
        timeAtLocation -= 24;
    if( (timeAtLocation >= 18 && timeAtLocation <= 24) ||
        (timeAtLocation >= 0 && timeAtLocation <= 5) )
        return "PM";
    else
        return "AM";
    return result;
}

QString WeatherForecastInfo::longitude() const {
    return weatherData().value(PROP_LONGITUDE,QString()).toString();
}

QString WeatherForecastInfo::latitude() const {
    return weatherData().value(PROP_LATITUDE,QString()).toString();
}

QString WeatherForecastInfo::windDirection() {
    return QString();
}

QString WeatherForecastInfo::windSpeed() {
    return weatherData().value(PROP_WINDSPEED,QString()).toString();
}

void WeatherForecastInfo::toogleTemperatureUnits() {
    QString currentUnits = temperatureUnits();
    QString targetUnits = (currentUnits.toLower() == "c")?("f"):("c");
    QString convertedTemp = temperatureForUnits(currentUnits,targetUnits);
    if(!convertedTemp.isEmpty()) {
        setWeatherData(PROP_TEMPERATUREUNITS,targetUnits.toUpper());
        setWeatherData(PROP_TEMPERATURE,convertedTemp);
        emit temperatureChanged();
        emit temperatureUnitsChanged();
    }
}

QString WeatherForecastInfo::temperatureForUnits(QString fromUnits,QString toUnits) {
    QString t = weatherData().value(PROP_TEMPERATURE,QString()).toString();
    if(!t.isEmpty() && fromUnits!=toUnits) {
        float temp = t.toFloat();
        if("c" == fromUnits.toLower() && "f" == toUnits.toLower())
            temp = (temp*1.8)+32;
        else if("f" == fromUnits.toLower() && "c" == toUnits.toLower())
            temp = (temp - 32)/1.8;
        t.setNum(temp,'f',1);
    }
    return t;
}

// eof
