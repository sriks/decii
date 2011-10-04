#ifndef WEATHERDATAHANDLER_H
#define WEATHERDATAHANDLER_H
#include <QObject>
#include <QVariant>

class WeatherDataHandlerPrivate;
class WeatherDataHandler : public QObject
{
    Q_OBJECT
public:
    explicit WeatherDataHandler(QObject *parent = 0);
    virtual ~WeatherDataHandler();
    Q_INVOKABLE QVariantMap weatherData() const;
    Q_INVOKABLE virtual bool isSupports(QString featureName) const;
public slots:
    virtual void handleNormalizedWeatherXml(QString normalizedXml);
    void printWeatherData() const;
protected:
    void setWeatherData(QString featureName,QVariant value);
private:
    void parseWeatherData(QByteArray data);
private:
    WeatherDataHandlerPrivate* d;
};
#endif // WEATHERDATAHANDLER_H
