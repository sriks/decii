#ifndef WEATHERFORECASTINFO_H
#define WEATHERFORECASTINFO_H
#include <QStringList>
#include "WeatherDataHandler.h"

class WeatherForecastInfoPrivate;
class WeatherForecastInfo : public WeatherDataHandler
{
    Q_OBJECT
    Q_PROPERTY(QString temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(QString temperatureUnits READ temperatureUnits NOTIFY temperatureUnitsChanged)
    Q_PROPERTY(QString condition READ condition NOTIFY conditionChanged)
    Q_PROPERTY(QString windDirection READ windDirection NOTIFY windDirectionChanged)
    Q_PROPERTY(QString windSpeed READ windSpeed NOTIFY windSpeedChanged)
    Q_PROPERTY(QString moonPhase READ moonPhase NOTIFY moonPhaseChanged)
    Q_PROPERTY(QString ampm READ ampm NOTIFY ampmChanged)

public:
    explicit WeatherForecastInfo(QObject *parent = 0);
    virtual ~WeatherForecastInfo();
public slots:
    Q_INVOKABLE virtual QString temperature();
    virtual QString temperatureUnits();
    Q_INVOKABLE virtual QString condition();
    Q_INVOKABLE virtual QString conditionImages();
    Q_INVOKABLE virtual QString windDirection();
    Q_INVOKABLE virtual QString windSpeed();
    Q_INVOKABLE virtual QString moonPhase() const;
    Q_INVOKABLE virtual QString ampm() const;
    Q_INVOKABLE virtual QString longitude() const;
    Q_INVOKABLE virtual QString latitude() const;
    virtual void toogleTemperatureUnits();
protected:
    virtual QString temperatureForUnits(QString fromUnits,QString toUnits);
signals:
    void temperatureChanged();
    void temperatureUnitsChanged();
    void conditionChanged();
    void windDirectionChanged();
    void windSpeedChanged();
    void moonPhaseChanged();
    void ampmChanged();

private:
    WeatherForecastInfoPrivate* d;
};

#endif // WEATHERFORECASTINFO_H
