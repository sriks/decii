#ifndef WEATHERMANAGER_H
#define WEATHERMANAGER_H

#include <QObject>
#include <QVariant>
class WeatherInfo;
class WeatherManagerPrivate;
class WeatherManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY emptyChanged)

public:
    explicit WeatherManager(QObject *parent = 0);
    virtual ~WeatherManager();
    Q_INVOKABLE QObject* suggestionProvider();
    Q_INVOKABLE int updateInterval() const;
    Q_INVOKABLE QString temperatureUnits() const;
    Q_INVOKABLE int maxSupportedLocations() const;
    Q_INVOKABLE bool canAddLocation() const;
signals:
    void emptyChanged();
    void countChanged();
    void stopUpdates();
    void restartUpdates(int interval);
    void showInfoBanner(QVariant text);
public slots:
    bool add(QObject* info);
    bool remove(int index);
    void removeAll();
    QObject* at(int index) const;
    int count() const;
    bool isEmpty() const;
    void handleError(QString error);
    bool online() const;
    void goOffline();
    void goOnline();
    void setTemperatureUnits(QString units);
    void setUpdateInterval(int interval);
protected slots:
    void appendToList(WeatherInfo* weatherInfo);
    bool isExists(QObject* info);
private:
    WeatherManagerPrivate* d;
};

#endif // WEATHERMANAGER_H
