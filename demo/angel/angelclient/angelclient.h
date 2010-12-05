#ifndef ANGELCLIENT_H
#define ANGELCLIENT_H

#include <QWidget>
#include <QAbstractSocket>
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

    void sendRequest(QByteArray aRequest);
private slots:
    QString readResponse(QString aSourceXml,QString aResponseType);

private:
    Ui::AngelClient *ui;
private:
    QTcpSocket* mClientSocket;
    QByteArray mCurrentRequest;
};

#endif // ANGELCLIENT_H
