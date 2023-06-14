#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "drivermodel.h"
#include "driverlist.h"
#include "vehiclemodel.h"
#include "vehiclelist.h"
#include "gascardmodel.h"
#include "gascardlist.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<DriverModel>("Driver", 1,0, "DriverModel");
    qmlRegisterUncreatableType<DriverList>("Driver", 1,0, "DriverList", QStringLiteral("DriverList should not be created in QML"));

    qmlRegisterType<VehicleModel>("Vehicle", 1,0, "VehicleModel");
    qmlRegisterUncreatableType<VehicleList>("Vehicle", 1,0, "VehicleList", QStringLiteral("VehicleList should not be created in QML"));

    qmlRegisterType<GascardModel>("Gascard", 1,0, "GascardModel");
    qmlRegisterUncreatableType<GascardList>("Gascard", 1,0, "GascardList", QStringLiteral("GascardList should not be created in QML"));

    DriverList driverList;
    VehicleList vehicleList;
    GascardList gascardList;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("driverList"), &driverList);
    engine.rootContext()->setContextProperty(QStringLiteral("vehicleList"), &vehicleList);
    engine.rootContext()->setContextProperty(QStringLiteral("gascardList"), &gascardList);

    const QUrl url(u"qrc:/v4/Main.qml"_qs);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
