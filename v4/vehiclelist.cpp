#include "vehiclelist.h"

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

VehicleList::VehicleList(QObject *parent)
    : QObject{parent}
{

}

QVector<VehicleItem> VehicleList::items() const
{
    return mItems;
}

bool VehicleList::setItemAt(int index, const VehicleItem &item)
{
    if (index < 0 || index >= mItems.size())
        return false;

    const VehicleItem &oldItem = mItems.at(index);
    if (item.vin == oldItem.vin)
        return false;

    mItems[index] = item;
    return true;
}

void VehicleList::fetchVehicles()
{
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QUrl url("https://affiche.me:7144/api/vehicles");
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

QString VehicleList::fetchDriverForVehicle(int driverID) {
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

void VehicleList::appendItem(const VehicleItem *item)
{
    emit preItemAppended();
    mItems.append(*item);
    emit postItemAppended();
}

void VehicleList::removeItems()
{
    for (int i=0;i < mItems.size(); ++i) {
        emit preItemRemoved(i);
        mItems.removeAt(i);
        emit postItemRemoved();
    }
}

void VehicleList::convertData(QString response)
{
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonArray jsonArr = doc.array();
    removeItems();
    for (int i = 0; i < jsonArr.size(); ++i) {
        VehicleItem item;
        QJsonObject vehicle = jsonArr.at(i).toObject();
        item.vin = vehicle.find("vin")->toString();
        item.brandModel = vehicle.find("brandModel")->toString();
        item.licensePlate = vehicle.find("licensePlate")->toString();
        item.driver = vehicle.find("driverId")->toInt();
        appendItem(&item);
    }
}

void VehicleList::setResponse(const QString &newResponse)
{
    if (_response == newResponse)
        return;
    _response = newResponse;
    convertData(_response);
}
