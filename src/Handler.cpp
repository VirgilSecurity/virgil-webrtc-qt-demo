#include "Handler.h"

#include <webrtc/api/peer_connection_interface.h>
#include <webrtc/pc/peer_connection.h>
#include <webrtc/pc/peer_connection_factory.h>

Handler::Handler(QObject *parent) : QObject(parent) {
}

void
Handler::call() {
    logMessage(QLatin1String("Calling..."));
}

void
Handler::answer() {
    logMessage(QLatin1String("Answering..."));
}

void
Handler::logMessage(const QString &message) {
    // Currently we don't need real logging, just emit a signal
    Q_EMIT messageLogged(message);
}
