#include <QtNetwork>
#include <QDebug>
#include <QXmlQuery>
#include <QBuffer>
#include "angelclient.h"
#include "ui_angelclient.h"
#include "qtsvgbutton.h"

const QString KXmlSource = "xmlsource";
const QString KResponseStatus = "status";
const QString KResponseText = "text";
const QString KResponseType = "responsetype";
const QString KXqReadResponse = "let $root := doc($xmlsource)//response return data($root/%1)";

// commands
const QByteArray KPlay          = "play";
const QByteArray KPause         = "pause";
const QByteArray KNext          = "next";
const QByteArray KPrev          = "prev";
const QByteArray KNowPlaying    = "nowplaying";
const QByteArray KTrackDuration = "trackduration";
const QByteArray KConnect       = "connect";

AngelClient::AngelClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AngelClient)
{
    const QString KSkin("MetallicBrush");
    mCurrentRequest.clear();
    ui->setupUi(this);
    ui->playPauseButton->setText(KPause);
//    ui->prevButton->setSkin(KSkin);
//    ui->nextButton->setSkin(KSkin);
//    ui->playPauseButton->setSkin(KSkin);

//    ui->hostAddressTextEdit->setPlainText(hostAddressToConnect());
//    ui->hostPortTextEdit->setPlainText("1500");
    connect(ui->bindButton,SIGNAL(clicked()),this,SLOT(connectToServer()));
    mClientSocket = new QTcpSocket(this);
    connect(mClientSocket,SIGNAL(connected()),this,SLOT(handleHostFound()));
    connect(mClientSocket,SIGNAL(readyRead()),this,SLOT(readServerResponse()));
    connect(mClientSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(handleError(QAbstractSocket::SocketError)));

    // connect player controls
    connect(ui->playPauseButton,SIGNAL(clicked()),this,SLOT(playPause()));
    connect(ui->nextButton,SIGNAL(clicked()),this,SLOT(next()));
    connect(ui->prevButton,SIGNAL(clicked()),this,SLOT(prev()));

    connectToServer();
}

AngelClient::~AngelClient()
{
    delete ui;
}

QString AngelClient::hostAddressToConnect()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            qDebug()<<"host address to connect "<<ipAddress;
            return ipAddress;
        }
    }
}

void AngelClient::handleError(QAbstractSocket::SocketError error)
{
    qDebug()<<__FUNCTION__;
    ui->connectionStatus->setText(mClientSocket->errorString());
}

void AngelClient::connectToServer()
{
    qDebug()<<__FUNCTION__;
    mCurrentRequest = KConnect;
    QHostAddress hostAddress(hostAddressToConnect());
    int port = 1500;
    mClientSocket->abort();
    mClientSocket->connectToHost(hostAddress,port);
}

void AngelClient::handleHostFound()
{

}

void AngelClient::readServerResponse()
{
    qDebug()<<__FUNCTION__;
    QDataStream in(mClientSocket);
    in.setVersion(QDataStream::Qt_4_0);
    in.device()->seek(0);
    QByteArray response = in.device()->readAll();
    int stat = readResponse(response,KResponseStatus).toInt();
    QString responseText = readResponse(response,KResponseText).simplified();

    if(200 != stat)
    {
        responseText = "Request failed!";
    }

    // TODO: Use a statemachine to handle it more elegantly
    else
    {
        if(KPlay == mCurrentRequest ||
           KNext == mCurrentRequest ||
           KPrev == mCurrentRequest)
        {
            sendRequest(KNowPlaying);
            ui->playingStatus->setText("Playing");
        }

        else if(KPause == mCurrentRequest)
        {
            ui->playingStatus->setText("Paused");
        }

        else if(KNowPlaying == mCurrentRequest)
        {
            ui->title->setText(responseText);
            sendRequest(KTrackDuration);
        }

        else if(KTrackDuration == mCurrentRequest)
        {
            ui->duration->setText(responseText);
        }

        else if(KConnect == mCurrentRequest)
        {
            ui->connectionStatus->setText(responseText);
            // Start playing after connection
            ui->playPauseButton->setText(KPlay);
            playPause();
        }
    }
}

QString AngelClient::readResponse(QString aSourceXml,QString aResponseType)
{
    QXmlQuery* xmlQuery = new QXmlQuery;
    QBuffer* buffer = new QBuffer(this);
    buffer->setData(aSourceXml.toUtf8());
    buffer->open(QIODevice::ReadOnly);
    xmlQuery->bindVariable(KXmlSource,buffer);
    QString query = KXqReadResponse.arg(aResponseType);
    xmlQuery->setQuery(query);
    QString result = QString();
    if(xmlQuery->isValid())
    {
       xmlQuery->evaluateTo(&result);
    }
    delete xmlQuery;
    return result;
}

void AngelClient::playPause()
{
    if(ui->playPauseButton->text() == KPlay)
    {
        sendRequest(KPlay);
        ui->playPauseButton->setText(KPause);
    }

    else if(ui->playPauseButton->text() == KPause)
    {
        sendRequest(KPause);
        ui->playPauseButton->setText(KPlay);
    }

}

void AngelClient::next()
{
    sendRequest(KNext);
}

void AngelClient::prev()
{
    sendRequest(KPrev);
}

void AngelClient::sendRequest(QByteArray aRequest)
{
    mCurrentRequest = aRequest;
    mClientSocket->write(aRequest);
}

// eof
