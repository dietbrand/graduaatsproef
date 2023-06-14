#include "vehiclemodel.h"
#include "vehiclelist.h"

VehicleModel::VehicleModel(QObject *parent)
    : QAbstractListModel(parent), mList(nullptr)
{
}
int VehicleModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !mList)
        return 0;
    return mList->items().size();
}

QVariant VehicleModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    const VehicleItem item = mList->items().at(index.row());

    switch(role) {
    case VinRole:
        return QVariant(item.vin);
    case LicensePlateRole:
        return QVariant(item.licensePlate);
    case BrandModelRole:
        return QVariant(item.brandModel);
    case DriverRole:
        return QVariant(item.driver);
    }

    return QVariant();
}

bool VehicleModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mList)
        return false;

    VehicleItem item = mList->items().at(index.row());

    switch(role) {
    case VinRole:
        item.vin = value.toString();
    case LicensePlateRole:
        item.licensePlate = value.toString();
    case BrandModelRole:
        item.brandModel = value.toString();
    case DriverRole:
        item.driver = value.toInt();
    }
    if (mList->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags VehicleModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> VehicleModel::roleNames() const
{
    QHash<int,QByteArray> names;
    names[VinRole] = "vin";
    names[LicensePlateRole] = "licensePlate";
    names[BrandModelRole] = "brandModel";
    names[DriverRole] = "driver";
    return names;
}

VehicleList *VehicleModel::list() const
{
    return mList;
}

void VehicleModel::setList(VehicleList *list)
{
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList) {
        connect(mList, &VehicleList::preItemAppended, this, [=]() {
            const int index = mList->items().size();
            beginInsertRows(QModelIndex(),index, index);
        });
        connect(mList, &VehicleList::postItemAppended, this, [=]() {
            endInsertRows();
        });
        connect(mList, &VehicleList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index,index);
        });
        connect(mList, &VehicleList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });
    }
    endResetModel();
}
