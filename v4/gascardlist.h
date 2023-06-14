#ifndef GASCARDLIST_H
#define GASCARDLIST_H

#include <QObject>

struct GascardItem {
    QString cardNumber;
    bool blocked;
    int pincode;
    int driver;
};

class GascardList : public QObject
{
    Q_OBJECT
public:
    explicit GascardList(QObject *parent = nullptr);
    QVector<GascardItem> items() const;

    bool setItemAt(int index, const GascardItem &item);

    Q_INVOKABLE void fetchGascards();
    Q_INVOKABLE QString fetchDriverForGascard(int driverID);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();
public slots:
    void appendItem(const GascardItem *item);
    void removeItems();
    void convertData(QString reply);
    void setResponse(const QString &newResponse);

private:
    QVector<GascardItem> mItems;
    QString _response;
};

#endif // GASCARDLIST_H
