#include "gascardmodel.h"
#include "gascardlist.h"

GascardModel::GascardModel(QObject *parent)
    : QAbstractListModel{parent},mList(nullptr)
{

}
int GascardModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !mList)
        return 0;
    return mList->items().size();
}

QVariant GascardModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    const GascardItem item = mList->items().at(index.row());

    switch(role) {
    case CardNumberRole:
        return QVariant(item.cardNumber);
    case BlockedRole:
        return QVariant(item.blocked);
    case PincodeRole:
        return QVariant(item.pincode);
    case DriverRole:
        return QVariant(item.driver);
    }

    return QVariant();
}

bool GascardModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mList)
        return false;

    GascardItem item = mList->items().at(index.row());

    switch(role) {
    case CardNumberRole:
        item.cardNumber = value.toString();
    case BlockedRole:
        item.blocked = value.toBool();
    case PincodeRole:
        item.pincode = value.toInt();
    case DriverRole:
        item.driver = value.toInt();
    }
    if (mList->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags GascardModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> GascardModel::roleNames() const
{
    QHash<int,QByteArray> names;
    names[CardNumberRole] = "cardNumber";
    names[BlockedRole] = "blocked";
    names[PincodeRole] = "pincode";
    names[DriverRole] = "driver";
    return names;
}

GascardList *GascardModel::list() const
{
    return mList;
}

void GascardModel::setList(GascardList *list)
{
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList) {
        connect(mList, &GascardList::preItemAppended, this, [=]() {
            const int index = mList->items().size();
            beginInsertRows(QModelIndex(),index, index);
        });
        connect(mList, &GascardList::postItemAppended, this, [=]() {
            endInsertRows();
        });
        connect(mList, &GascardList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index,index);
        });
        connect(mList, &GascardList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });
    }
    endResetModel();
}
