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
const QString KXqReadOperation = "let $root:=doc($xmlfilename)//commands for $operation in $root/operation where $operation/@id=$id return $operation";
const QString KXqReadOptions = "let $root:=doc($source)//operation return data($root/option)";
const QString KLinuxCommandFileName = "linux_commands.xml";

Server::Server(QWidget *parent)
:   QDialog(parent), tcpServer(0), networkSession(0),
    mIsLastRequestSuccess(false)
{
    statusLabel = new QLabel;
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);

    mProcess = new QProcess(this);
    mXmlQuery = new QXmlQuery;
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }

    //! [2]
        fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
                 << tr("You've got to think about tomorrow.")
                 << tr("You will be surprised by a loud noise.")
                 << tr("You will feel hungry again in another hour.")
                 << tr("You might have mail.")
                 << tr("You cannot kill time without injuring eternity.")
                 << tr("Computers are not intelligent. They only think they are.");
    //! [2]

        connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    //! [3]
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(sendFortune()));
    //! [3]

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addStretch(1);
        buttonLayout->addWidget(quitButton);
        buttonLayout->addStretch(1);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(statusLabel);
        mainLayout->addLayout(buttonLayout);
        setLayout(mainLayout);

        setWindowTitle(tr("Fortune Server"));
}

Server::~Server()
{
    delete mXmlQuery;
}

void Server::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

//! [0] //! [1]
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any,1234)) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }
//! [0]
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
//! [1]
}

//! [4]
void Server::sendFortune()
{

    QByteArray response = "Conneted Successfully";
    mCommandForPlayer = commandForPlayer("rhythmbox");

    if(mCommandForPlayer.isEmpty())
    {
        response = "no command for player found";
    }



//    QDataStream out(&block, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_4_0);
////! [4] //! [6]
////    out << (quint16)0;
////    out << fortunes.at(qrand() % fortunes.size());
////    out.device()->seek(0);
////    out << (quint16)(block.size() - sizeof(quint16));
////! [6] //! [7]

//    out.device()->write("test");
//    out.device()->seek(0);
    mClientConnection = tcpServer->nextPendingConnection();
    connect(mClientConnection, SIGNAL(disconnected()),
            mClientConnection, SLOT(deleteLater()));

    connect(mClientConnection,SIGNAL(readyRead()),this,SLOT(handleCommand()));
//! [7] //! [8]

    mClientConnection->write(response);
//    clientConnection->disconnectFromHost();
//! [5]
}
//! [8]

void Server::handleCommand()
{
    QString requestFromClient = mClientConnection->readAll();
    qDebug()<<"requestFromClient: "<<requestFromClient;

    if("islastcommandsuccess" == requestFromClient)
    {
        QByteArray response = (mIsLastRequestSuccess)?("true"):("false");
        mClientConnection->write(response);
        return;
    }

    mIsLastRequestSuccess = false;
    if("next" == requestFromClient)
    {
        qDebug()<<mCommandForPlayer;
        QString operation = operationDetails(requestFromClient);
        operation = operation.trimmed();
        QString opt = option(operation);
        qDebug()<<opt;
        QString commandToExecute = mCommandForPlayer+opt;
        qDebug()<<"commandToExecute:"<<commandToExecute;

        mProcess->setStandardOutputFile("stdout.txt");
        int stat = mProcess->execute(commandToExecute);
        mIsLastRequestSuccess = !stat; // on success return is 0
        qDebug()<<"stdout:"<<mProcess->readAllStandardError();
        qDebug()<<stat;
        qDebug()<<mProcess->exitStatus();
        qDebug()<<mProcess->exitCode();
    }
}

bool Server::isProcessSuccess(QProcess* aProcess)
{
}

QString Server::commandForPlayer(QString aPlayerName)
{
    QXmlQuery* xmlQuery = new QXmlQuery;
#ifdef Q_OS_LINUX
    xmlQuery->bindVariable(KXmlFileName,QVariant(KLinuxCommandFileName));
#endif
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

QString Server::operationDetails(QString aId)
{
#ifdef Q_OS_LINUX
    mXmlQuery->bindVariable(KXmlFileName,QVariant(KLinuxCommandFileName));
#endif
    mXmlQuery->bindVariable(KId,QVariant(aId));
    mXmlQuery->setQuery(KXqReadOperation);
    QString result = QString();
    if(mXmlQuery->isValid())
    {
       mXmlQuery->evaluateTo(&result);
    }
    qDebug()<<result;
    return result;
}

QString Server::option(QString aOperation)
{
    QByteArray tmp = aOperation.toUtf8();
    QBuffer buffer(&tmp);
    buffer.open(QIODevice::ReadOnly|QIODevice::Text);
    QXmlQuery* xmlQuery = new QXmlQuery;
    xmlQuery->bindVariable(KSource,&buffer);
    xmlQuery->setQuery(KXqReadOptions);
    QString result = QString();
    if(xmlQuery->isValid())
    {
        xmlQuery->evaluateTo(&result);
    }
    result = result.trimmed();
    qDebug()<<result;
    delete xmlQuery;
    buffer.close();
    return result;
}

//eof
