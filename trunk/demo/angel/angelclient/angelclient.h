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
    void handleReadyRead();
    void connectToServer();
    void handleError(QAbstractSocket::SocketError error);
private:
    Ui::AngelClient *ui;
private:
    QTcpSocket* mClientSocket;
    quint16 blockSize;
};

#endif // ANGELCLIENT_H
