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

void VehicleList::fetchVehicleData()
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
        // rapporteer fout
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
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
        QJsonObject driver = jsonArr.at(i).toObject();
        item.vin = driver.find("vin")->toString();
        item.brandModel = driver.find("brandModel")->toString();

        item.licensePlate = driver.find("licensePlate")->toString();
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
