#ifndef HOROSCOPE_H
#define HOROSCOPE_H

#include <QtGui/QWidget>
#include <QHttp>
#include <QLabel>
#include <QListWidget>
#include <QComboBox>
#include <QGraphicsWebView>
#include <QState>

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
    // Required to move window around since we have a frameless window
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

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
    /*void on_showselection_entered();
*/

private slots:
    void on_http_requestFinished (int id,bool error);
    // this will emit selection changed signal
    void on_selectionwebview_linkClicked(const QUrl& aSelectedUrl);
    void on_webview_linkClicked(const QUrl & url);

private:
    // States
    QState* iGroupState;
    QState* iGetSettingsState;
    QState* iHttpRequestState;
    QState* iDisplayContentState;
    QState* iDisplaySelectionState;

    QGraphicsWebView* iSelectionWebView;
    QHttp* iHttp;
    QPoint dragPosition;
    QString iCurrentZodiacSign;
    QString iPrevHtmlContent;
    QByteArray iCurrentResponse;
    HoroscopeData iHoroscopeData;
};

#endif // HOROSCOPE_H
