#ifndef VEHICLEMODEL_H
#define VEHICLEMODEL_H

#include <QAbstractListModel>

class VehicleList;

class VehicleModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(VehicleList *list READ list WRITE setList NOTIFY listChanged)

public:
    explicit VehicleModel(QObject *parent = nullptr);

    enum {
        VinRole = Qt::UserRole,
        LicensePlateRole,
        BrandModelRole,
        DriverRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int,QByteArray> roleNames() const override;

    VehicleList *list() const;
    void setList(VehicleList *list);

signals:
    void listChanged();

    void responseChanged();

private:
    VehicleList *mList;
};

#endif // VEHICLEMODEL_H
