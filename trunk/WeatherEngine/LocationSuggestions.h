#ifndef LOCATIONSUGGESTIONS_H
#define LOCATIONSUGGESTIONS_H

#include <QObject>
#include <QStringList>
#include <QVariant>

class SuggestionInfo : public QObject {
    Q_OBJECT
public:
    QString _title;
    QString _subTitle;
    QVariant _info;
public:
    Q_INVOKABLE QString title() const { return _title; }
    Q_INVOKABLE QString subTitle() const { return _subTitle; }
    Q_INVOKABLE QVariant info() const { return _info; }
    bool operator ==(const SuggestionInfo& other) const;
    virtual ~SuggestionInfo();
public slots:
    bool save(QString filename);
    bool restore(QString filename);
    void print() const;
};

class LocationSuggestions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit LocationSuggestions(QObject *parent = 0);
    Q_INVOKABLE virtual void handleInput(QString userTyping);
public slots:
    virtual int count() const;
    virtual void done();
signals:
    void countChanged();
};

#endif // LOCATIONSUGGESTIONS_H
