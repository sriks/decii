#ifndef WEATHERINFO_H
#define WEATHERINFO_H

#include <QObject>
#include "WeatherForecastInfo.h"
class WeatherInfoPrivate;
class WeatherInfo: public WeatherForecastInfo
{
    Q_OBJECT
    Q_PROPERTY(QString lastUpdatedTime READ lastUpdatedTime NOTIFY lastUpdatedTimeChanged)

public:
    explicit WeatherInfo(QObject *parent = 0);
    virtual ~WeatherInfo();

    Q_INVOKABLE virtual QObject* info() const;
    Q_INVOKABLE virtual QString title() const;
    Q_INVOKABLE virtual QString subTitle() const;
    Q_INVOKABLE virtual QString locationId() const;
    Q_INVOKABLE virtual QObject* forecast(int numOfdays);
    Q_INVOKABLE virtual QString serviceProviderName() const;
    Q_INVOKABLE virtual QString serviceProviderUrl() const;
    Q_INVOKABLE virtual QString lastUpdatedTime() const;

public slots:
    virtual void update()=0;
    virtual void setInfo(QObject* info);
protected slots:
    virtual void updateLastUpdatedTime();
signals:
    void updated(WeatherInfo* info);
    void error(QString);
    void lastUpdatedTimeChanged();
private:
    WeatherInfoPrivate* d;
};

#endif // WEATHERINFO_H
