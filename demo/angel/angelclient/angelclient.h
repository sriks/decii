#ifndef ANGELCLIENT_H
#define ANGELCLIENT_H

#include <QWidget>
#include <QAbstractSocket>
#include <QTime>
#include <QBasicTimer>
namespace Ui {
    class AngelClient;
}

class QTcpSocket;
class QAbstractSocket;
class AngelClient : public QWidget
{
    Q_OBJECT

public:
    explicit AngelClient(QWidget *parent = 0);
    ~AngelClient();

public slots:
    void handleHostFound();
    void readServerResponse();
    void connectToServer();
    void handleError(QAbstractSocket::SocketError error);
    QString hostAddressToConnect();

    void playPause();
    void next();
    void prev();
    void trackDuration();
    void trackPosition();
    void sendRequest(QByteArray aRequest);

private slots:
    void setupNetworkSession();
    QString readResponse(QString aSourceXml,QString aResponseType);
    void sliderValueChanged(int aNewValue);
    void updateElapsedTime();

private:
    int timeInSecs(QString aTimeInText);
    void timerEvent(QTimerEvent *aEvent);

private:
    Ui::AngelClient *ui;
private:
    QTcpSocket* mClientSocket;
    QByteArray mCurrentRequest;
    int mTrackDurationInSec;
    QTime mTrackElapsedTime;
    bool mHasHourPart;
    bool mIsPaused;
    int mSyncTimerId;
    QBasicTimer mTrackTimer;
};

#endif // ANGELCLIENT_H
