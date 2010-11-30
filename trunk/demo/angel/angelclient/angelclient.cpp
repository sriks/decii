#include <QtNetwork>
#include <QDebug>
#include "angelclient.h"
#include "ui_angelclient.h"

AngelClient::AngelClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AngelClient)
{
    ui->setupUi(this);
    connect(ui->bindButton,SIGNAL(clicked()),this,SLOT(connectToServer()));
    mClientSocket = new QTcpSocket(this);
    connect(mClientSocket,SIGNAL(connected()),this,SLOT(handleHostFound()));
    connect(mClientSocket,SIGNAL(readyRead()),this,SLOT(handleReadyRead()));
    connect(mClientSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(handleError(QAbstractSocket::SocketError)));
}

AngelClient::~AngelClient()
{
    delete ui;
}

void AngelClient::handleError(QAbstractSocket::SocketError error)
{
    qDebug()<<__FUNCTION__;
    qDebug()<<error;
    qDebug()<<mClientSocket->errorString();
    ui->statusLabel->setText(mClientSocket->errorString());

}

void AngelClient::connectToServer()
{
    qDebug()<<__FUNCTION__;
    QHostAddress hostAddress(ui->hostAddressTextEdit->toPlainText());
    int port = ui->hostPortTextEdit->toPlainText().toInt();
    blockSize = 0;
    mClientSocket->abort();
    mClientSocket->connectToHost(hostAddress,port);
}

void AngelClient::handleHostFound()
{
    ui->statusLabel->setText("Host found");
    qDebug()<<"connected";
}

void AngelClient::handleReadyRead()
{
    qDebug()<<__FUNCTION__;
    QDataStream in(mClientSocket);
    in.setVersion(QDataStream::Qt_4_0);
    int bytes = mClientSocket->bytesAvailable();
    qDebug()<<bytes;
//    if (blockSize == 0)
//    {
//        if (mClientSocket->bytesAvailable() < (int)sizeof(quint16))
//        {
//            qDebug()<<"Invalid bytes available";
//            return;
//        }
//        in >> blockSize;
//    }

//    if (mClientSocket->bytesAvailable() < blockSize)
//    {
//        qDebug()<<"Invalid block size available";
//        return;
//    }

//    qDebug()<<blockSize;
    QByteArray nextFortune;
    in.device()->seek(0);
    nextFortune = in.device()->readAll();
    qDebug()<<nextFortune;
    ui->msgLabel->setText(nextFortune);
}

// eof
