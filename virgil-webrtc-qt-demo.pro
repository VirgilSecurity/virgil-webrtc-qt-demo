QT += quick

CONFIG += c++14 qtquickcompiler
TARGET = virgil-webrtc-demo

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    include/Handler.h

SOURCES += \
    src/main.cpp \
    src/Handler.cpp

RESOURCES += \
    src/ui/qml.qrc

INCLUDEPATH += include

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
