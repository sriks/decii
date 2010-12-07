/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QtNetwork>
#include <QDebug>
#include <QProcess>
#include <stdlib.h>
#include <QXmlQuery>
#include "server.h"

const QString KXmlFileName = "xmlfilename";
const QString KId = "id";
const QString KSource = "source";
const QString KPlayer = "player";
const QString KXqReadCommandForPlayer = "let $root:=doc($xmlfilename)//commands where $root/@player=$player return data($root/@command)";
const QString KXqReadOption = "let $root:=doc($xmlfilename)//commands for $operation in $root/operation where $operation/@id=$id return data($operation/option)";
const QString KLinuxCommandFileName = "linux_commands.xml";
const QString KWindowsCommandFileName = "win_commands.xml";
const QString KRhythmbox = "rhythmbox";
const QString KWinamp    = "winamp";
const QString KNowPlaying    = "nowplaying";
const QString KPlay          = "play";
const QString KSyncNow       = "syncnow";
const QString KResponseTemplate = "<response><status>%1</status><text>%2</text></response>";

const int KStatusSuccess =  200;
const int KStatusInternalError = 500;
const int KOneSecondInMs = 1000;

Server::Server(QWidget *parent)
:   QDialog(parent), tcpServer(0), networkSession(0),
    mInternalSync(false)
{
    statusLabel = new QLabel;
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);
    mCurrentTrackName.clear();
    mProcess = new QProcess(this);
    mXmlQuery = new QXmlQuery;
    connect(mProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)));

// Populate command filename depending on the underlying platform
    QString commandsFileName;
#ifdef Q_OS_LINUX
    commandsFileName = ":/xml/linux_commands.xml";
    // TODO: Add support to get the player name dynamically
    mPlayerName = KRhythmbox;
#else Q_OS_WIN32
    commandsFileName = ":/xml/win_commands.xml";;
    mPlayerName = KWinamp;
#endif

    mCommands = new QFile(commandsFileName,this);
    if(!mCommands->open(QIODevice::ReadOnly))
    {

    }

    openSession();
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
    setWindowTitle(tr("Angel Server"));
}

Server::~Server()
{
    delete mXmlQuery;
}

void Server::openSession()
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any,1500)) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Fortune Client example now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));
}

void Server::handleNewConnection()
{
    mCommandForPlayer = commandForPlayer(mPlayerName);
    QByteArray response = "Conneted Successfully : "+mPlayerName.toLocal8Bit();

    if(mCommandForPlayer.isEmpty())
    {
        response = "No supporting player!";
    }
    mClientConnection = tcpServer->nextPendingConnection();
    connect(mClientConnection, SIGNAL(disconnected()),
            mClientConnection, SLOT(deleteLater()));

    connect(mClientConnection,SIGNAL(readyRead()),this,SLOT(handleRequest()));
    sendResponse(KStatusSuccess,response);

    // start sync timer
    startTimer(KOneSecondInMs*4);
}

void Server::handleRequest()
{
    QString requestFromClient = mClientConnection->readAll();
    qDebug()<<"requestFromClient: "<<requestFromClient;
    mCurrentRequest = requestFromClient.simplified();
    executeCommand(requestFromClient);
}

void Server::executeCommand(QString aRequest)
{
    QString opt = option(aRequest);
    QString commandToExecute = mCommandForPlayer+opt;
    commandToExecute = commandToExecute.simplified();
    qDebug()<<"commandToExecute:"<<commandToExecute;
    mProcess->start(commandToExecute);
}

void Server::processFinished (int exitCode,QProcess::ExitStatus exitStatus)
{
qDebug()<<__FUNCTION__;
    QString response = mProcess->readAllStandardOutput().simplified();

    // check if sycn is required
    if(mInternalSync)
    {
        mInternalSync = false;
        qDebug()<<"sync required: "<<mCurrentTrackName<<" "<<response;
        if(mCurrentTrackName != response)
        {
        mCurrentTrackName = response;
        sync();
        }

    }

    else
    {
    int stat = (0 == exitCode)?(KStatusSuccess):(KStatusInternalError);
    sendResponse(stat,response);
    }
}

void Server::readProcessOutput()
{
    qDebug()<<__FUNCTION__;
    qDebug()<<mProcess->exitStatus();
    qDebug()<<mProcess->exitCode();
}

void Server::timerEvent(QTimerEvent *event)
{
    checkIsSyncRequired();
}

void Server::checkIsSyncRequired()
{
    // get track title
    mInternalSync = true;
    executeCommand(KNowPlaying);
}

void Server::sync()
{
    sendResponse(KStatusSuccess,KSyncNow);
}

QString Server::commandForPlayer(QString aPlayerName)
{
    mCommands->reset();
    QXmlQuery* xmlQuery = new QXmlQuery;
    xmlQuery->bindVariable(KXmlFileName,mCommands);

    xmlQuery->bindVariable(KPlayer,QVariant(aPlayerName));
    xmlQuery->setQuery(KXqReadCommandForPlayer);
    QString result = QString();
    if(xmlQuery->isValid())
    {
        xmlQuery->evaluateTo(&result);
    }
    delete xmlQuery;
    return result;
}

QString Server::option(QString aId)
{
    mCommands->reset();
    mXmlQuery->bindVariable(KXmlFileName,mCommands);
    mXmlQuery->bindVariable(KId,QVariant(aId));
    mXmlQuery->setQuery(KXqReadOption);
    QString result = QString();
    if(mXmlQuery->isValid())
    {
       mXmlQuery->evaluateTo(&result);
    }
    qDebug()<<result;
    return result;
}

void Server::sendResponse(int aStatus, QString aResponseText)
{
    // TODO: add request string in response
    qDebug()<<__FUNCTION__;
    QString resp = KResponseTemplate.arg(aStatus).arg(aResponseText);
    qDebug()<<resp;
    mClientConnection->write(resp.toUtf8());
}

//eof
