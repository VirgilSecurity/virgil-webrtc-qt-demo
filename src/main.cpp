#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "EchoCall.h"
#include "voip/Call.h"
#include "voip/CallError.h"

int
main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qRegisterMetaType<virgil::voip::CallPhase>("CallPhase");
    qRegisterMetaType<virgil::voip::CallConnectionState>("CallConnectionState");
    qRegisterMetaType<virgil::voip::CallError>("CallError");

    EchoCall handler(&app);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("handler", &handler);
    engine.load(QStringLiteral("qrc:/main.qml"));
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "QmlEngine root objects are empty";
        app.exit(-1);
    }

    return app.exec();
}
