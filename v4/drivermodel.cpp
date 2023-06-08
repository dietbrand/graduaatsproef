#include "drivermodel.h"
#include "driverlist.h"

DriverModel::DriverModel(QObject *parent)
    : QAbstractListModel(parent), mList(nullptr)
{

}

int DriverModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !mList)
        return 0;
    return mList->items().size();
}

QVariant DriverModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    const DriverItem item = mList->items().at(index.row());

    switch(role) {
    case IdRole:
        return QVariant(item.id);
    case FullnameRole:
        return QVariant(item.fullname);
    case BirthDateRole:
        return QVariant(item.birthDate);
    case RRNRole:
        return QVariant(item.rrn);
    }

    return QVariant();
}

bool DriverModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mList)
        return false;

    DriverItem item = mList->items().at(index.row());

    switch(role) {
    case IdRole:
        item.id = value.toInt();
    case FullnameRole:
        item.fullname = value.toString();
    case BirthDateRole:
        item.birthDate = value.toDate();
    case RRNRole:
        item.rrn = value.toString();
    }
    if (mList->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags DriverModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> DriverModel::roleNames() const
{
    QHash<int,QByteArray> names;
    names[IdRole] = "id";
    names[FullnameRole] = "fullname";
    names[BirthDateRole] = "birthDate";
    names[RRNRole] = "rrn";
    return names;
}

DriverList *DriverModel::list() const
{
    return mList;
}

void DriverModel::setList(DriverList *list)
{
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList) {
        connect(mList, &DriverList::preItemAppended, this, [=]() {
            const int index = mList->items().size();
            beginInsertRows(QModelIndex(),index, index);
        });
        connect(mList, &DriverList::postItemAppended, this, [=]() {
            endInsertRows();
        });
        connect(mList, &DriverList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index,index);
        });
        connect(mList, &DriverList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });        
    }
    endResetModel();
}
