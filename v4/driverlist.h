#ifndef DRIVERLIST_H
#define DRIVERLIST_H

#include <QObject>
#include <QDate>

struct DriverItem
{
    int id;
    QString fullname;
    QString rrn;
    QDate birthDate;
};

class DriverList : public QObject
{
    Q_OBJECT
public:
    explicit DriverList(QObject *parent = nullptr);

    QVector<DriverItem> items() const;

    bool setItemAt(int index, const DriverItem &item);

    Q_INVOKABLE void fetchDrivers();

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem(const DriverItem *item);
    void removeItems();
    void convertData(QString reply);
    void setResponse(const QString &newResponse);

private:
    QVector<DriverItem> mItems;
    QString _response;
};

#endif // DRIVERLIST_H
