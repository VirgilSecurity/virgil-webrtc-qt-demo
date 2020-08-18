QT += quick websockets

CONFIG += c++17 qtquickcompiler no_keywords
TARGET = virgil-webrtc-demo

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    include/EchoCall.h \
    include/utils/quuid_unordered_map_spec.h \
    include/voip/Call.h \
    include/voip/CallAnswer.h \
    include/voip/CallConnectionFactory.h \
    include/voip/CallError.h \
    include/voip/CallException.h \
    include/voip/CallManager.h \
    include/voip/CallOffer.h \
    include/voip/CallSignalingMessage.h \
    include/voip/IceCandidate.h \
    include/voip/IncomingCall.h \
    include/voip/OutgoingCall.h \
    src/queue/dispatch_queue.h \
    src/voip/Observers.h \
    src/voip/PeerConnectionQueue.h

SOURCES += \
    src/EchoCall.cpp \
    src/main.cpp \
    src/queue/dispatch_queue.cpp \
    src/voip/Call.cpp \
    src/voip/CallAnswer.cpp \
    src/voip/CallConnectionFactory.cpp \
    src/voip/CallException.cpp \
    src/voip/CallManager.cpp \
    src/voip/CallOffer.cpp \
    src/voip/CallSignalingMessage.cpp \
    src/voip/IceCandidate.cpp \
    src/voip/IncomingCall.cpp \
    src/voip/Observers.cpp \
    src/voip/OutgoingCall.cpp \
    src/voip/PeerConnectionQueue.cpp

RESOURCES += \
    src/ui/qml.qrc

INCLUDEPATH += include src

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 3rdparty
macx {
    message("* Using settings for MacOS.")
    LIBS += -L$$PWD/3rdparty/WebRTC/mac/lib -lwebrtc_d
    LIBS += -framework Foundation -framework CoreServices -framework ApplicationServices -framework CoreAudio -framework AudioToolbox
    DEFINES += WEBRTC_MAC WEBRTC_POSIX WEBRTC_UNIX
    WEBRTC_LIB_SUBDIR = "mac"
}

linux:!android {
    message("* Using settings for Linux.")
    LIBS += -L$$PWD/3rdparty/WebRTC/linux/lib/x86_64 -lwebrtc_d
    LIBS += -framework Foundation -framework CoreServices -framework ApplicationServices -framework CoreAudio -framework AudioToolbox
    DEFINES += WEBRTC_MAC WEBRTC_POSIX WEBRTC_UNIX
    WEBRTC_LIB_SUBDIR = "linux"
}

android {
    message("* Using settings for Android.")
    LIBS += -L$$PWD/3rdparty/WebRTC/android/lib/$$ANDROID_TARGET_ARCH -lwebrtc_d
    LIBS += -landroid -lOpenSLES
    DEFINES += WEBRTC_LINUX WEBRTC_ANDROID WEBRTC_POSIX
    SOURCES += src/jni/init_android.cpp
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
    ANDROID_MIN_SDK_VERSION = 26
    ANDROID_TARGET_SDK_VERSION = 26
    ANDROID_API_VERSION = 8
    WEBRTC_LIB_SUBDIR = "android"
}

ios {
    message("* Using settings for iOS.")
    QMAKE_INFO_PLIST = $$PWD/ios/Info.plist
    QMAKE_LFLAGS += -force_load $$PWD/3rdparty/WebRTC/ios/lib/libwebrtc_d.a
    LIBS += -framework Foundation -framework CoreServices -framework CoreAudio -framework AudioToolbox -framework AVFoundation -framework CoreMedia
    DEFINES += WEBRTC_MAC WEBRTC_IOS WEBRTC_POSIX WEBRTC_UNIX
    WEBRTC_LIB_SUBDIR = "ios"
}

INCLUDEPATH += \
    $$PWD/3rdparty \
    $$PWD/3rdparty/WebRTC/$$WEBRTC_LIB_SUBDIR/include \
    $$PWD/3rdparty/WebRTC/$$WEBRTC_LIB_SUBDIR/include/webrtc \
    $$PWD/3rdparty/WebRTC/$$WEBRTC_LIB_SUBDIR/include/webrtc/third_party \
    $$PWD/3rdparty/WebRTC/$$WEBRTC_LIB_SUBDIR/include/webrtc/third_party/abseil-cpp

DEPENDPATH += $$PWD/3rdparty/WebRTC/$$WEBRTC_LIB_SUBDIR/include
