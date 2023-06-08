#include "driverlist.h"

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

DriverList::DriverList(QObject *parent)
    : QObject{parent}
{

}

QVector<DriverItem> DriverList::items() const
{
    return mItems;
}

bool DriverList::setItemAt(int index, const DriverItem &item)
{
    if (index < 0 || index >= mItems.size())
        return false;

    const DriverItem &oldItem = mItems.at(index);
    if (item.id == oldItem.id)
        return false;

    mItems[index] = item;
    return true;
}

void DriverList::fetchDriverData()
{
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QUrl url("https://affiche.me:7144/api/drivers");
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

void DriverList::appendItem(const DriverItem *item)
{
    emit preItemAppended();
    mItems.append(*item);
    emit postItemAppended();
}

void DriverList::removeItems()
{
    for (int i=0;i < mItems.size(); ++i) {
        emit preItemRemoved(i);
        mItems.removeAt(i);
        emit postItemRemoved();
    }
}

void DriverList::convertData(QString response)
{
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonArray jsonArr = doc.array();
    removeItems();
    for (int i = 0; i < jsonArr.size(); ++i) {
        DriverItem item;
        QJsonObject driver = jsonArr.at(i).toObject();
        item.id = driver.find("driverID")->toInt();
        item.rrn = driver.find("natRegNum")->toString();

        QString fullDate = driver.find("birthDate")->toString().split('T')[0];
        item.birthDate = QDate::fromString(fullDate,"yyyy-MM-dd");

        item.fullname = driver.find("firstName")->toString() + " " + driver.find("lastName")->toString();
        appendItem(&item);
    }
}

void DriverList::setResponse(const QString &newResponse)
{
    if (_response == newResponse)
        return;
    _response = newResponse;
    convertData(_response);
}
