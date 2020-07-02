#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "Handler.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    Handler handler(&app);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("handler", &handler);
    engine.load(QStringLiteral("qrc:/main.qml"));
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "QmlEngine root objects are empty";
        app.exit(-1);
    }

    return app.exec();
}
