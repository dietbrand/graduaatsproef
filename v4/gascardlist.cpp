#include "gascardlist.h"

#include <QVector>
#include <QDebug>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

GascardList::GascardList(QObject *parent)
    : QObject{parent}
{

}
QVector<GascardItem> GascardList::items() const
{
    return mItems;
}

bool GascardList::setItemAt(int index, const GascardItem &item)
{
    if (index < 0 || index >= mItems.size())
        return false;

    const GascardItem &oldItem = mItems.at(index);
    if (item.cardNumber == oldItem.cardNumber)
        return false;

    mItems[index] = item;
    return true;
}

void GascardList::fetchGascards()
{
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QUrl url("https://affiche.me:7144/api/gascards");
    QNetworkRequest req(url);
    QNetworkReply *reply = mgr.get(req);
    reply->ignoreSslErrors();
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        setResponse(reply->readAll());
        delete reply;
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
    }
}

QString GascardList::fetchDriverForGascard(int driverID) {
    if (driverID == 0) {
        return "";
    }
    try {
        QEventLoop eventLoop;
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        QUrl url("https://affiche.me:7144/api/drivers/" + QString::number(driverID));
        QNetworkRequest req(url);
        QNetworkReply *reply = mgr.get(req);
        reply->ignoreSslErrors();
        eventLoop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject driver = doc.object();
            QString firstName = driver.find("firstName")->toString();
            QString lastName = driver.find("lastName")->toString();
            return firstName + " " + lastName;
        }
        else {
            throw (reply->errorString());
        }
        delete reply;
    } catch (QString err){
        return err;
    }
}

void GascardList::appendItem(const GascardItem *item)
{
    emit preItemAppended();
    mItems.append(*item);
    emit postItemAppended();
}

void GascardList::removeItems()
{
    for (int i=0;i < mItems.size(); ++i) {
        emit preItemRemoved(i);
        mItems.removeAt(i);
        emit postItemRemoved();
    }
}

void GascardList::convertData(QString response)
{
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonArray jsonArr = doc.array();
    removeItems();
    for (int i = 0; i < jsonArr.size(); ++i) {
        GascardItem item;
        QJsonObject gascard = jsonArr.at(i).toObject();
        item.cardNumber = gascard.find("cardNumber")->toString();
        item.blocked = gascard.find("blocked")->toBool();
        item.pincode = gascard.find("pincode")->toInt();
        item.driver = gascard.find("driverId")->toInt();
        appendItem(&item);
    }
}

void GascardList::setResponse(const QString &newResponse)
{
    if (_response == newResponse)
        return;
    _response = newResponse;
    convertData(_response);
}
