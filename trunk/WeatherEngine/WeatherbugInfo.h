#ifndef WEATHERBUGINFO_H
#define WEATHERBUGINFO_H
#include <QNetworkReply>
#include "WeatherInfo.h"
#include "LocationSuggestions.h"
class WeatherBugInfoPrivate;
class WeatherBugInfo : public WeatherInfo
{
    Q_OBJECT
public:
    explicit WeatherBugInfo(QObject *parent = 0);
    ~WeatherBugInfo();
public slots:
    virtual void update();
    virtual void setInfo(QObject* info);
    virtual QObject* info() const;
private slots:
    void onDataAvailable(QByteArray data);
    void onNetworkError(QNetworkReply::NetworkError errorType,QString errorString);
private:
    WeatherBugInfoPrivate* d;
};

class WeatherBugLocationSuggestionsPrivate;
class WeatherBugLocationSuggestions : public LocationSuggestions
{
    Q_OBJECT
public:
    WeatherBugLocationSuggestions(QObject* parent=0);
    ~WeatherBugLocationSuggestions();
    Q_INVOKABLE virtual void handleInput(QString userTyping);
    Q_INVOKABLE QObject* suggestionInfo(int index);
public slots:
    virtual int count() const;
    virtual void done();
private:
    void clearSuggestions();
    WeatherBugLocationSuggestionsPrivate* d;
};

#endif // WEATHERBUGINFO_H
