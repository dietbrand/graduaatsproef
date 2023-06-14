#ifndef GASCARDMODEL_H
#define GASCARDMODEL_H

#include <QAbstractListModel>

class GascardList;

class GascardModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(GascardList *list READ list WRITE setList NOTIFY listChanged)
public:
    explicit GascardModel(QObject *parent = nullptr);

    enum {
        CardNumberRole = Qt::UserRole,
        BlockedRole,
        PincodeRole,
        DriverRole,
    };
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int,QByteArray> roleNames() const override;

    GascardList *list() const;
    void setList(GascardList *list);

signals:
    void listChanged();
    void responseChanged();

private:
    GascardList *mList;
};

#endif // GASCARDMODEL_H
