// Author: Srikanth Sombhatla

#ifndef HOROSCOPE_H
#define HOROSCOPE_H

#include <QtGui/QWidget>
#include <QGraphicsWebView>

#ifdef Q_OS_SYMBIAN
    #include "S60QHttp.h"
#else
    #include <QHttp>
#endif

/*!
  Horoscope graphics view widget.
  Holds a state machine and derives from GraphicsWebView
  */

struct HoroscopeData
{
    QString iDescription;
    QString iZodiacSign;
    QString iDate;
};

class QGraphicsItem;
class QGraphicsWebView;
class QState;
class Horoscope : public QGraphicsWebView
{
    Q_OBJECT

public:
    Horoscope(QGraphicsItem * parent = 0);
    ~Horoscope();

public:
    // Required to exit upon Esc
    void keyPressEvent(QKeyEvent*);

Q_SIGNALS:
    // Transition signals
    //
    void initiateNetworkRequest();
    void networkResponseRecieved();
    void displayContentReady();
    void selectionCancelClicked();
    void selectionChanged();

private slots:
    // State Entered slots
    // All slots follow auto connection syntax with corresponding state's objectName
    void on_getsettings_entered();
    void on_httprequest_entered();
    void on_displaycontent_entered();

private slots:
    void on_http_requestFinished (int id,bool error);
    void on_webview_linkClicked(const QUrl & url);
    void updateMessage(const QString& aMessage);
private:
    // States
    QState* iGroupState;
    QState* iGetSettingsState;
    QState* iHttpRequestState;
    QState* iDisplayContentState;
    QState* iDisplaySelectionState;

#ifdef Q_OS_SYMBIAN
    S60QHttp* iHttp;
#else
    QHttp* iHttp;
#endif

    QPoint dragPosition;
    QString iCurrentZodiacSign;
    QString iPrevHtmlContent;
    QByteArray iCurrentResponse;
    HoroscopeData iHoroscopeData;
};

#endif // HOROSCOPE_H
