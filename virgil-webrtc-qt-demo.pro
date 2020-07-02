QT += quick

CONFIG += c++14 qtquickcompiler
TARGET = virgil-webrtc-demo

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    Handler.h

SOURCES += \
    main.cpp \
    Handler.cpp

RESOURCES += qml.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
