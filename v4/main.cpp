#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "drivermodel.h"
#include "driverlist.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<DriverModel>("Driver", 1,0, "DriverModel");
    qmlRegisterUncreatableType<DriverList>("Driver", 1,0, "DriverList", QStringLiteral("DriverList should not be created in QML"));

    DriverList driverList;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("driverList"), &driverList);

    const QUrl url(u"qrc:/v4/Main.qml"_qs);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
