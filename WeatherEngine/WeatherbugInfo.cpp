#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QNetworkRequest>
#include <QFile>
#include <QBuffer>
#include <QVariantMap>
#include "LocationSuggestions.h"
#include "WeatherbugInfo.h"
#include "NetworkEngine.h"
#include "XQueryEngine.h"
#include "Constants.h"

QString WeatherRequestAPI("http://api.wxbug.net/getLiveWeatherRSS.aspx?ACode=A5580663883&unittype=1&outputtype=1&");
QString USALocation("0");
QString NonUSALocation("1");
QString Zipcode("zipcode");
QString Citycode("citycode");
QString Citytype("citytype");


struct WeatherBugInfoPrivate {
    NetworkEngine* net;
    XQueryEngine* xquery;
    SuggestionInfo* info;
    WeatherBugInfoPrivate() {
        net = new NetworkEngine;
        xquery = new XQueryEngine;
        info = NULL;
    }

    ~WeatherBugInfoPrivate() {
        net->deleteLater();
        xquery->deleteLater();
    }
};

WeatherBugInfo::WeatherBugInfo(QObject *parent) :
    WeatherInfo(parent) {
    d = new WeatherBugInfoPrivate;
    connect(d->net,SIGNAL(dataAvailable(QByteArray)),this,SLOT(onDataAvailable(QByteArray)));
    connect(d->net,SIGNAL(error(QNetworkReply::NetworkError,QString)),
            this,SLOT(onNetworkError(QNetworkReply::NetworkError,QString)));
}

WeatherBugInfo::~WeatherBugInfo() {
    delete d;
}

void WeatherBugInfo::update() {
    if(!d->info)
        return;
    SuggestionInfo* si = qobject_cast<SuggestionInfo*>(d->info);
    if(si) {
        si->setParent(this);
        QVariantMap extraInfo = si->info().toMap();
        QString additionalParams;
        if(USALocation == extraInfo.value(Citytype).toString())
            additionalParams = Zipcode+"="+extraInfo.value(Zipcode).toString();
        else
            additionalParams = Citycode+"="+extraInfo.value(Citycode).toString();
        QUrl url(WeatherRequestAPI+additionalParams);
        qDebug()<<Q_FUNC_INFO<<url;
        QNetworkRequest request(url);
        d->net->get(request);
    } else {
        qWarning()<<Q_FUNC_INFO<<"invalid info";
    }

}

void WeatherBugInfo::onDataAvailable(QByteArray data) {
    QBuffer b(&data);
    QFile f(":/resources/weatherbug/converttonormalizedxml.xq");
    if(f.open(QIODevice::ReadOnly) && b.open(QIODevice::ReadOnly)) {
        QString q = f.readAll();
        QVariant result = d->xquery->executeQuery(q,&b);
        if(result.isValid() && d->xquery->isSuccess()) {
            handleNormalizedWeatherXml(result.toString());
            // Some times WB sends invaid city names, hence using what user has selected.
            setWeatherData(PROP_CITY,d->info->title());
            setWeatherData(PROP_COUNTRY,d->info->subTitle());
            setWeatherData(PROP_TEMPERATUREUNITS,"C"); // default is C
            // convert percentage of moonphase to number.
            int moonPhase = weatherData().value(PROP_MOONPHASE).toString().toInt();
            if(moonPhase > 30) {
                int moonPhaseNum = (moonPhase*30)/100;
                setWeatherData(PROP_MOONPHASE,QVariant(QString().setNum(moonPhaseNum)));
            }
            qDebug()<<Q_FUNC_INFO<<weatherData();
            emit updated(this);
        } else {
            emit error(QString("Unable to get weather data."));
        }
    }
    f.close();
}

void WeatherBugInfo::onNetworkError(QNetworkReply::NetworkError errorType, QString errorString) {
    Q_UNUSED(errorType)
    Q_UNUSED(errorString);
    emit error("Network conection error");
}

void WeatherBugInfo::setInfo(QObject* info) {
    if(info) {
        info->setParent(this);
        d->info = qobject_cast<SuggestionInfo*>(info);
    }
}

QObject * WeatherBugInfo::info() const {
    return d->info;
}

// WeatherBugLocationSuggestions
QString KSuggFilterRegex("(\\b%1)\\w+\\b");
struct WeatherBugLocationSuggestionsPrivate {
    QMap<QString,QObject*> names;
    QStringList suggestions;
    QString query;
    XQueryEngine* xquery;
    WeatherBugLocationSuggestionsPrivate() {
        xquery = new XQueryEngine;
    }
    ~WeatherBugLocationSuggestionsPrivate() {
        xquery->deleteLater();
    }
};

WeatherBugLocationSuggestions::WeatherBugLocationSuggestions(QObject *parent) {
    d = new WeatherBugLocationSuggestionsPrivate;
    QFile xqf(":/resources/weatherbug/locationnames/names.xq");
    if(xqf.open(QIODevice::ReadOnly))
        d->query = xqf.readAll();
    xqf.close();
}

WeatherBugLocationSuggestions::~WeatherBugLocationSuggestions() {
    delete d;
}

/*!
  Handles input from user and returns location suggestions.
**/
void WeatherBugLocationSuggestions::handleInput(QString userTyping) {
    QString key = userTyping.trimmed().toLower();
    if(1 == key.size()) {
        QString firstChar = key.at(0);
        QFile data(":/resources/weatherbug/locationnames/"+firstChar+".txt");
          if(data.open(QIODevice::ReadOnly)) {
             clearSuggestions();
             QStringList* sl = new QStringList(QString(data.readAll()).split("\n"));
             QStringListIterator iter(*sl);
             while(iter.hasNext()) {
                 QStringList info = iter.next().split("|");
                 if(info.size() != 6)
                     continue;
                 SuggestionInfo* sugginfo = new SuggestionInfo;
                 sugginfo->setParent(this);
                 sugginfo->_title = info.at(0);
                 sugginfo->_subTitle = info.at(1) + " " + info.at(2); // statename country name
                 sugginfo->_subTitle = sugginfo->_subTitle.trimmed();
                 QVariantMap extra;
                 extra.insert(Zipcode,info.at(3));
                 extra.insert(Citycode,info.at(4));
                 extra.insert(Citytype,info.at(5));
                 sugginfo->_info = QVariant(extra);
                 d->names.insert(sugginfo->_title+sugginfo->_subTitle,sugginfo);
             }
             delete sl;
          }
          data.close();
    } else if(0 == key.size()) {
        d->suggestions.clear();
    }

    QString regex = KSuggFilterRegex.arg(key);
    d->suggestions = QStringList(d->names.keys()).filter(QRegExp(regex,Qt::CaseInsensitive));
    emit countChanged();
}

int WeatherBugLocationSuggestions::count() const {
    return d->suggestions.size();
}

QObject * WeatherBugLocationSuggestions::suggestionInfo(int index) {
    if(index >=0 && index < d->suggestions.size())
        return d->names.value(d->suggestions.at(index));
    else {
        qWarning()<<Q_FUNC_INFO<<"accessing invalid index "<<index<<" sugg count is:"<<count();
        return NULL;
    }
}

void WeatherBugLocationSuggestions::done() {
    clearSuggestions();
}

void WeatherBugLocationSuggestions::clearSuggestions() {
    if(!d->names.isEmpty()) {
        QMapIterator<QString,QObject*> iter(d->names);
        while(iter.hasNext()) {
            QObject* sugg = iter.next().value();
            if(sugg->parent() == this)
                sugg->deleteLater();
        }
        d->names.clear();
    }
}

// eof
