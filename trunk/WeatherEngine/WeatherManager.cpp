#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QSettings>
#include <QApplication>
#include "WeatherManager.h"
#include "LocationSuggestions.h"
#include "WeatherFactory.h"
#include "WeatherInfo.h"

const int KMaxSupportedLocations(6);
const int KMinute = 60*1000;
const int KDefaultUpdateInterval = 60*KMinute;
const QString KDefaultTempUnits = "C";
const QString PREF("preferences");
const QString PREF_UPDATEINTERVAL("updateinterval");
const QString PREF_TEMPUNITS("tempunits");
const QString PREF_ONLINE("online");

struct WeatherManagerPrivate {
    QVariantMap preferences;
    QList<WeatherInfo*> weatherServiceProviders;
    WeatherManagerPrivate() {
        QSettings s(QApplication::organizationName(),QApplication::applicationName());
        preferences = s.value(PREF,QVariantMap()).toMap();
        if(preferences.isEmpty()) {
            // Populate with default values
            preferences.insert(PREF_UPDATEINTERVAL,QVariant(KDefaultUpdateInterval));
            preferences.insert(PREF_TEMPUNITS,QVariant(KDefaultTempUnits));
            preferences.insert(PREF_ONLINE,QVariant(true));
        }
    }

    void backup() {
        QSettings s(QApplication::organizationName(),QApplication::applicationName());
        s.setValue(PREF,preferences);
        QListIterator<WeatherInfo*> iter(weatherServiceProviders);
        int count = 0;
        while(iter.hasNext()) {
            SuggestionInfo* si = qobject_cast<SuggestionInfo*>(iter.next()->info());
            if(si)
                si->save(QString().setNum(++count)+".si"); //si: suggestioninfo
        }
    }

    ~WeatherManagerPrivate() {
        QStringList listToDelete = QDir().entryList(QStringList("*.si"),QDir::Files,QDir::Name);
        QStringListIterator deliter(listToDelete);
        while(deliter.hasNext())
            QDir().remove(deliter.next());
        if(weatherServiceProviders.size()) {
            backup();
            qDeleteAll(weatherServiceProviders.begin(),weatherServiceProviders.end());
            weatherServiceProviders.clear();
        }
    }
};

WeatherManager::WeatherManager(QObject *parent) :
                QObject(parent) {
    d = new WeatherManagerPrivate;

    // restore if any
    QStringList filter("*.si");
    QStringList savedInfoList = QDir().entryList(filter,QDir::Files,QDir::Name);
    QStringListIterator iter(savedInfoList);
    while(iter.hasNext()) {
        SuggestionInfo* info = new SuggestionInfo;
        if(info->restore(iter.next()))
            add(info);
        else
            info->deleteLater();
    }
}

WeatherManager::~WeatherManager() {
    delete d;
}

bool WeatherManager::add(QObject* info) {
    if(isExists(info))
        return false;
    WeatherInfo* w = qobject_cast<WeatherInfo*>(WeatherFactory::weatherEngine());
    if(w) {
        w->setParent(this);
        connect(w,SIGNAL(updated(WeatherInfo*)),this,SLOT(appendToList(WeatherInfo*)),Qt::UniqueConnection);
        connect(w,SIGNAL(updated(WeatherInfo*)),this,SIGNAL(emptyChanged()),Qt::UniqueConnection);
        connect(w,SIGNAL(updated(WeatherInfo*)),this,SIGNAL(countChanged()),Qt::UniqueConnection);
        connect(w,SIGNAL(error(QString)),this,SLOT(handleError(QString)),Qt::UniqueConnection);
        connect(this,SIGNAL(restartUpdates(int)),w,SLOT(update()));
        w->setInfo(info);
        if(online())
            w->update();
    }
    return true;
}

void WeatherManager::appendToList(WeatherInfo *weatherInfo) {
    if(weatherInfo) {
        int index = d->weatherServiceProviders.indexOf(weatherInfo);
        if(index >= 0)
            d->weatherServiceProviders.replace(index,weatherInfo);
        else
            d->weatherServiceProviders.append(weatherInfo);
        QTimer* t = new QTimer(weatherInfo);
        t->setInterval(updateInterval());
        connect(t,SIGNAL(timeout()),weatherInfo,SLOT(update()),Qt::UniqueConnection);
        connect(this,SIGNAL(restartUpdates(int)),t,SLOT(start(int)),Qt::UniqueConnection);
        connect(this,SIGNAL(stopUpdates()),t,SLOT(stop()));
        t->start();
    }
}

/*!
  Returns true if this info is already added.
  Expects info which is SuggestionInfo object.
**/
bool WeatherManager::isExists(QObject *info) {
    bool exists = false;
    SuggestionInfo* infoToCheck = qobject_cast<SuggestionInfo*>(info);
    if(infoToCheck) {
        QListIterator<WeatherInfo*> iter(d->weatherServiceProviders);
        while(iter.hasNext()) {
            SuggestionInfo* existingInfo = qobject_cast<SuggestionInfo*>(iter.next()->info());
            if(existingInfo && *existingInfo == *infoToCheck) {
                exists = true;
                break;
            }
        }
    }
    return exists;
}

bool WeatherManager::remove(int index) {
    if(index >=0 && index < count()) {
        d->weatherServiceProviders.at(index)->deleteLater();
        d->weatherServiceProviders.removeAt(index);
        emit countChanged();
        emit emptyChanged();
    }
}

void WeatherManager::removeAll() {
    int cnt = count();
    for(int i=cnt-1;i>=0;--i)
        remove(i);
}

QObject * WeatherManager::suggestionProvider() {
    QObject* sp = WeatherFactory::suggestionProvider();
    if(sp)
        sp->setParent(this);
    return sp;
}

int WeatherManager::count() const {
    return d->weatherServiceProviders.size();
}

bool WeatherManager::isEmpty() const {
    return (count())?(false):(true);
}

QObject * WeatherManager::at(int index) const {
    return d->weatherServiceProviders.at(index);
}

void WeatherManager::handleError(QString error) {
    emit showInfoBanner(error);
}

void WeatherManager::goOffline() {
    if(!online())
        return;
    d->preferences.insert(PREF_ONLINE,false);
    emit stopUpdates();
}

void WeatherManager::goOnline() {
    if(online())
        return;
    d->preferences.insert(PREF_ONLINE,true);
    emit restartUpdates(updateInterval());
}

bool WeatherManager::online() const {
    return d->preferences.value(PREF_ONLINE,true).toBool();
}

void WeatherManager::setTemperatureUnits(QString units) {
    d->preferences.insert(PREF_TEMPUNITS,units);
}

void WeatherManager::setUpdateInterval(int interval) {
    if(updateInterval() != interval)
       d->preferences.insert(PREF_UPDATEINTERVAL,interval);
}

int WeatherManager::updateInterval() const {
    return d->preferences.value(PREF_UPDATEINTERVAL,KDefaultUpdateInterval).toInt();
}

QString WeatherManager::temperatureUnits() const {
    return d->preferences.value(PREF_TEMPUNITS,KDefaultTempUnits).toString();
}

int WeatherManager::maxSupportedLocations() const {
    return KMaxSupportedLocations;
}

bool WeatherManager::canAddLocation() const {
    return (count() < maxSupportedLocations());
}


// eof
