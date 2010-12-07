#include <QtNetwork>
#include <QDesktopWidget>
#include <QDebug>
#include "angelclient.h"
#include "ui_angelclient.h"
#include "qtsvgbutton.h"

const QString KXmlSource = "xmlsource";
const QString KResponseStatus = "status";
const QString KResponseText = "text";
const QString KResponseType = "responsetype";
const QString KXqReadResponse = "let $root := doc($xmlsource)//response return data($root/%1)";
const int KOneSecondInMs = 1000;

// commands
const QByteArray KPlay          = "play";
const QByteArray KPause         = "pause";
const QByteArray KNext          = "next";
const QByteArray KPrev          = "prev";
const QByteArray KNowPlaying    = "nowplaying";
const QByteArray KTrackDuration = "trackduration";
const QByteArray KTrackPosition = "trackposition";
const QByteArray KConnect       = "connect";
const QByteArray KSyncNow       = "syncnow";

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
    const QString KSkin("Beryl");
    mCurrentRequest.clear();
    ui->setupUi(this);
    setLayout(ui->masterLayout);

#ifdef Q_OS_SYMBIAN
    QDesktopWidget dw;
    QRect screenRect = dw.screenGeometry();
    resize(screenRect.size());
#else
    resize(250,400);
#endif

    ui->playPauseButton->setSkin(KSkin);
    ui->nextButton->setSkin(KSkin);
    ui->prevButton->setSkin(KSkin);
    ui->playPauseButton->setText(KPause);
    ui->slider->setTracking(false);
    mXmlQuery = new QXmlQuery;
    mBuffer = new QBuffer(this);

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
#ifdef Q_OS_SYBIAN
    setDefaultIap();
#endif

    ui->hostAddress->setText(hostAddressToConnect());
    ui->port->setText("1500");
    connectToServer();
}

AngelClient::~AngelClient()
{
    delete ui;
    delete mXmlQuery;
}

QString AngelClient::hostAddressToConnect()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        qDebug()<<"ipaddress:"<<ipAddressesList.at(i);
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
    qDebug()<<"errornum:"<<error;
    ui->connectionStatus->setText(QString().setNum(error) + " " + mClientSocket->errorString());
}

void AngelClient::connectToServer()
{
    qDebug()<<__FUNCTION__;
    mCurrentRequest = KConnect;
    QHostAddress hostAddress(ui->hostAddress->text());
    int port = ui->port->text().toInt();
    mClientSocket->abort();
    mClientSocket->connectToHost(hostAddress,port);
}

void AngelClient::handleHostFound()
{
    qDebug()<<__FUNCTION__;
    ui->console->setText("connected to host");
}

void AngelClient::readServerResponse()
{
    qDebug()<<__FUNCTION__;
    ui->console->clear();
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

    else if(KSyncNow == responseText)
    {
        sync();
    }

    // TODO: Use a statemachine to handle it more elegantly
    else
    {
        if(KPlay == mCurrentRequest ||
           KNext == mCurrentRequest ||
           KPrev == mCurrentRequest)
        {
            if(KPlay == mCurrentRequest)
            {
                ui->playPauseButton->setEnabled(true);
                ui->playPauseButton->setText(KPause);
            }
            sendRequest(KNowPlaying);
            ui->playingStatus->setText("Playing");
        }

        else if(KPause == mCurrentRequest)
        {
            ui->playPauseButton->setEnabled(true);
            ui->playPauseButton->setText(KPlay);
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
            trackPosition();
        }

        else if(KTrackPosition == mCurrentRequest)
        {
            qDebug()<<"updated position";
            int secs = timeInSecs(responseText);
            ui->slider->setRange(0,mTrackDurationInSec);
            ui->slider->setValue(secs);
            mTrackTimer.stop();
            mTrackTimer.start(KOneSecondInMs,this);
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
    mBuffer->setData(aSourceXml.toUtf8());
    mBuffer->open(QIODevice::ReadOnly);
    mXmlQuery->bindVariable(KXmlSource,mBuffer);
    QString query = KXqReadResponse.arg(aResponseType);
    mXmlQuery->setQuery(query);
    QString result = QString();
    if(mXmlQuery->isValid())
    {
       mXmlQuery->evaluateTo(&result);
    }
    mBuffer->close();
    return result;
}

void AngelClient::playPause()
{
    if(ui->playPauseButton->text() == KPlay)
    {
        sendRequest(KPlay);
        ui->playPauseButton->setEnabled(false);
        mIsPaused = false;
    }

    else if(ui->playPauseButton->text() == KPause)
    {
        sendRequest(KPause);
        ui->playPauseButton->setEnabled(false);
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
    return result;
    }
return -1;
}

void AngelClient::timerEvent(QTimerEvent *aEvent)
{
    if(!mIsPaused)
    {
        ui->slider->setValue(ui->slider->value()+1);
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

void AngelClient::sync()
{
    mTrackTimer.stop();
    sendRequest(KNowPlaying);
    ui->console->setText("syncing...");
}

void AngelClient::sendRequest(QByteArray aRequest)
{
    mCurrentRequest = aRequest;
    mClientSocket->write(aRequest);
}
// eof
