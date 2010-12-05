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
const int KOneSecondInMs = 1000+100;

// commands
const QByteArray KPlay          = "play";
const QByteArray KPause         = "pause";
const QByteArray KNext          = "next";
const QByteArray KPrev          = "prev";
const QByteArray KNowPlaying    = "nowplaying";
const QByteArray KTrackDuration = "trackduration";
const QByteArray KTrackPosition = "trackposition";
const QByteArray KConnect       = "connect";


#ifdef Q_OS_SYMBIAN
#include <es_sock.h>
#include <sys/socket.h>
#include <net/if.h>

static void setDefaultIapL()
    {
    RSocketServ serv;
    CleanupClosePushL(serv);
    User::LeaveIfError(serv.Connect());

    RConnection conn;
    CleanupClosePushL(conn);
    User::LeaveIfError(conn.Open(serv));
    User::LeaveIfError(conn.Start());

    _LIT(KIapNameSetting, "IAP\\Name");
    TBuf8<50> iap8Name;

    User::LeaveIfError(conn.GetDesSetting(TPtrC(KIapNameSetting), iap8Name));
    iap8Name.ZeroTerminate();

    conn.Stop();
    CleanupStack::PopAndDestroy(&conn);
    CleanupStack::PopAndDestroy(&serv);

    struct ifreq ifReq;
    memset(&ifReq, 0, sizeof(struct ifreq));
    strcpy( ifReq.ifr_name, (char*)iap8Name.Ptr());
    User::LeaveIfError(setdefaultif( &ifReq ));
}

static int setDefaultIap()
{
    TRAPD(err, setDefaultIapL());
    qDebug()<<"Error in setDefaultIap: "<<err;
    return err;
}
#endif


AngelClient::AngelClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AngelClient)
{
    const QString KSkin("MetallicBrush");
    mCurrentRequest.clear();
    ui->setupUi(this);
    ui->playPauseButton->setText(KPause);
//    ui->slider->setSingleStep(1);
//    ui->slider->setMinimum(0);
//    ui->slider->setMaximum(0);

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
    connect(ui->slider,SIGNAL(valueChanged(int)),this,SLOT(sliderValueChanged(int)));
    setDefaultIap();
    connectToServer();
}

AngelClient::~AngelClient()
{
    delete ui;
}

void AngelClient::setupNetworkSession()
{
//QNetworkConfigurationManager manager;
//if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
//    // Get saved network configuration
//    QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
//    settings.beginGroup(QLatin1String("QtNetwork"));
//    const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
//    settings.endGroup();

//    // If the saved network configuration is not currently discovered use the system default
//    QNetworkConfiguration config = manager.configurationFromIdentifier(id);
//    if ((config.state() & QNetworkConfiguration::Discovered) !=
//        QNetworkConfiguration::Discovered) {
//        config = manager.defaultConfiguration();
//    }

//    networkSession = new QNetworkSession(config, this);
//    connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

//    getFortuneButton->setEnabled(false);
//    statusLabel->setText(tr("Opening network session."));
//    networkSession->open();
//}
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
            trackDuration();
        }

        else if(KTrackDuration == mCurrentRequest)
        {
            mTrackDurationInSec = timeInSecs(responseText);
            ui->duration->setText(responseText);
//            this->startTimer(KOneSecondInMs);
            mTrackTimer.start(KOneSecondInMs,this);
            mSyncTimerId = this->startTimer(KOneSecondInMs*4);
            trackPosition();
        }

        else if(KTrackPosition == mCurrentRequest)
        {
            ui->console->setText(responseText);
            int secs = timeInSecs(responseText);
            ui->slider->setRange(0,mTrackDurationInSec);
            ui->slider->setValue(secs);
            mTrackElapsedTime.setHMS(0,0,secs,0);
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
        mIsPaused = false;
    }

    else if(ui->playPauseButton->text() == KPause)
    {
        sendRequest(KPause);
        ui->playPauseButton->setText(KPlay);
        mIsPaused = true;
        mTrackTimer.stop();
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

void AngelClient::trackDuration()
{
    sendRequest(KTrackDuration);
}

void AngelClient::trackPosition()
{
    sendRequest(KTrackPosition);
}

int AngelClient::timeInSecs(QString aTimeInText)
{
    qDebug()<<aTimeInText;
    QString timeInText = aTimeInText.simplified();
    QStringList timeList = timeInText.split(":");
    qDebug()<<timeList<<timeList.count();
    mHasHourPart = false;
    if(2 >= timeList.count() && aTimeInText.contains(":"))
    {
        int hr = 0;
        int index = 0;
        if(3 == timeList.count()) // has hour part also
        {
            mHasHourPart = true;
            hr = timeList.at(index).toInt();
            ++index;
        }

    int min = timeList.at(index).toInt();
    index++;
    int sec = timeList.at(index).toInt();
    int result = (hr*60)*60 + min*60 + sec;
    qDebug()<<hr<<" "<<min<<" "<<sec<<" in sec:"<<result;
    return result;
    }
return -1;
}

void AngelClient::timerEvent(QTimerEvent *aEvent)
{
    if(aEvent->timerId() == mSyncTimerId)
    {
        sendRequest(KNowPlaying);
        ui->console->setText("syncing");
    }
    else
    {
        if(!mIsPaused)
        {
            ui->slider->setValue(ui->slider->value()+1);
        }
    }
}

void AngelClient::sliderValueChanged(int aNewValue)
{
    qDebug()<<__FUNCTION__;
    qDebug()<<aNewValue;
    if(!mIsPaused && aNewValue <= mTrackDurationInSec)
    {
        updateElapsedTime();
    }
}

void AngelClient::updateElapsedTime()
{
    if(!mIsPaused)
    {
        mTrackElapsedTime = mTrackElapsedTime.addSecs(1);
        QString elapsedTimeText;
        elapsedTimeText = mTrackElapsedTime.toString("mm") + ":" +
                          mTrackElapsedTime.toString("ss");

        if(mHasHourPart)
        {
            elapsedTimeText.insert(0,mTrackElapsedTime.toString("hh")+":");
        }
        ui->elapsed->setText(elapsedTimeText);
    }

}


void AngelClient::sendRequest(QByteArray aRequest)
{
    mCurrentRequest = aRequest;
    mClientSocket->write(aRequest);
}

// eof
