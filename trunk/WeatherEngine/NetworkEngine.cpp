#include "NetworkEngine.h"

NetworkEngine::NetworkEngine(QObject *parent) :
    QObject(parent) {
    nam = new QNetworkAccessManager(this);
    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyAvailable(QNetworkReply*)));
}

//void NetworkEngine::post(QNetworkRequest *request) {
//    nam->post(request);
//}

void NetworkEngine::get(QNetworkRequest& request) {
    nam->get(request);
}

void NetworkEngine::replyAvailable(QNetworkReply *reply) {
    if(QNetworkReply::NoError == reply->error())
        emit dataAvailable(reply->readAll());
    else
        emit error(reply->error(),reply->errorString());
    reply->deleteLater();
}

// eof
