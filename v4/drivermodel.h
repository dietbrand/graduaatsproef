#ifndef DRIVERMODEL_H
#define DRIVERMODEL_H

#include <QAbstractListModel>

class DriverList;

class DriverModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(DriverList *list READ list WRITE setList NOTIFY listChanged)

public:
    explicit DriverModel(QObject *parent = nullptr);

    enum {
        IdRole = Qt::UserRole,
        FullnameRole,
        BirthDateRole,
        RRNRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int,QByteArray> roleNames() const override;

    DriverList *list() const;
    void setList(DriverList *list);

signals:
    void listChanged();

    void responseChanged();

private:
    DriverList *mList;
};

#endif // DRIVERMODEL_H
