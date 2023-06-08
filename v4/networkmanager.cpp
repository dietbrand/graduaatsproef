#include "networkmanager.h"
#include <QNetworkReply>
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}
{
    _manager = new QNetworkAccessManager(this);
}

void NetworkManager::doGetRequest(const QString &url)
{
    auto _request = QScopedPointer<QNetworkRequest>(new QNetworkRequest());
    _request->setUrl(url);
//    _request->setTransferTimeout(1000);
    _request->setRawHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:99.0) Gecko/20100101 Firefox/99.0");
    _request->setRawHeader("Content-Type", "application/json");    
    QNetworkReply *reply = _manager->get(*_request);
    reply->ignoreSslErrors();
    QObject::connect(reply, &QNetworkReply::finished, this, &NetworkManager::slotFinished);    
}

void NetworkManager::slotFinished()
{
    QNetworkReply *reply = dynamic_cast<QNetworkReply*>(sender());

    if(reply != nullptr) {
        // Hier hebben we pas toegang tot de data

        setResponse(reply->readAll());
        qDebug() << response();
        reply->deleteLater();
    }
}
const QString &NetworkManager::response() const
{
    return _response;
}

void NetworkManager::setResponse(const QString &newResponse)
{
    if (_response == newResponse)
        return;
    _response = newResponse;
    emit responseChanged(_response);
}
