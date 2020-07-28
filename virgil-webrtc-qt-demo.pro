QT += quick

CONFIG += c++17 qtquickcompiler no_keywords
TARGET = virgil-webrtc-demo

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    include/Handler.h \
    include/voip/Call.h \
    include/voip/CallAnswer.h \
    include/voip/CallConnectionFactory.h \
    include/voip/CallError.h \
    include/voip/CallException.h \
    include/voip/CallOffer.h \
    include/voip/CallSignalingMessage.h \
    include/voip/OutgoingCall.h \
    src/voip/Observers.h

SOURCES += \
    src/main.cpp \
    src/Handler.cpp \
    src/voip/Call.cpp \
    src/voip/CallAnswer.cpp \
    src/voip/CallConnectionFactory.cpp \
    src/voip/CallException.cpp \
    src/voip/CallOffer.cpp \
    src/voip/CallSignalingMessage.cpp \
    src/voip/Observers.cpp \
    src/voip/OutgoingCall.cpp

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
