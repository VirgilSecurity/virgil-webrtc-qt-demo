QT += quick

CONFIG += c++14 qtquickcompiler no_keywords
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

# WebRTC
macx: LIBS += -L$$PWD/3rdparty/WebRTC/lib -lwebrtc -framework Foundation -framework CoreServices -framework ApplicationServices -framework CoreAudio
macx: DEFINES += WEBRTC_MAC WEBRTC_POSIX

INCLUDEPATH += \
    $$PWD/3rdparty/WebRTC/include \
    $$PWD/3rdparty/WebRTC/include/webrtc \
    $$PWD/3rdparty/WebRTC/include/third_party \
    $$PWD/3rdparty/WebRTC/include/third_party/abseil-cpp

DEPENDPATH += $$PWD/3rdparty/WebRTC/include
