#ifndef VEHICLELIST_H
#define VEHICLELIST_H

#include <QObject>

struct VehicleItem {
    QString vin;
    QString licensePlate;
    QString brandModel;
    int driver;
};

class VehicleList : public QObject
{
    Q_OBJECT
public:
    explicit VehicleList(QObject *parent = nullptr);
    QVector<VehicleItem> items() const;

    bool setItemAt(int index, const VehicleItem &item);

    Q_INVOKABLE void fetchVehicles();
    Q_INVOKABLE QString fetchDriverForVehicle(int driverID);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem(const VehicleItem *item);
    void removeItems();
    void convertData(QString reply);
    void setResponse(const QString &newResponse);

private:
    QVector<VehicleItem> mItems;
    QString _response;
};

#endif // VEHICLELIST_H
