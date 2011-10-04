#include <QDebug>
#include <QFile>
#include <QVariantMap>
#include "LocationSuggestions.h"

LocationSuggestions::LocationSuggestions(QObject *parent) :
    QObject(parent) {}

void LocationSuggestions::handleInput(QString userTyping) {
    Q_UNUSED(userTyping)
}

int LocationSuggestions::count() const { return 0; }

bool SuggestionInfo::save(QString filename) {
    print();
    bool result = false;
    QVariantMap dataToStore; // TODO make this variantmap as the datastructure for this class.
    dataToStore.insert("title",QVariant(title()));
    dataToStore.insert("subtitle",QVariant(subTitle()));
    dataToStore.insert("info",QVariant(info()));

    QFile file(filename);
    if(file.open(QIODevice::WriteOnly)) {
        result = true;
        QDataStream out(&file);
        out << QVariant(dataToStore);
        file.close();
    }
    return result;
}

bool SuggestionInfo::restore(QString filename) {
    bool success = true;
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        QVariant data;
        in >> data;
        if(data.isValid()) {
            QVariantMap dataMap = data.toMap();
            _title = dataMap.value("title",QString()).toString();
            _subTitle = dataMap.value("subtitle",QString()).toString();
            _info = dataMap.value("info",QVariantMap()).toMap();
            print();
        }
        else {
            success = false;
        }
    }
    return success;
}

SuggestionInfo::~SuggestionInfo() {}

void SuggestionInfo::print() const {
    qDebug()<<__PRETTY_FUNCTION__<<"\ntitle:"<<title()<<"\nsubtitle:"<<subTitle()<<"\ninfo:"<<info();
}

bool SuggestionInfo::operator ==(const SuggestionInfo &other) const {
    return (other.title() == this->title() && other.subTitle() == this->subTitle());
}

void LocationSuggestions::done() {}

// eof
