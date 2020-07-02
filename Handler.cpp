#include "Handler.h"

Handler::Handler(QObject *parent)
    : QObject(parent)
{}

void Handler::call()
{
    logMessage(QLatin1String("Calling..."));
}

void Handler::answer()
{
    logMessage(QLatin1String("Answering..."));
}

void Handler::logMessage(const QString &message)
{
    // Currently we don't need real logging, just emit a signal
    emit messageLogged(message);
}
