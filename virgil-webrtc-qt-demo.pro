QT += quick websockets

CONFIG += c++17 qtquickcompiler no_keywords
TARGET = virgil-webrtc-demo

clang {
    message("* Using compiler: clang.")
    QMAKE_CXXFLAGS += -Wthread-safety
}

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    include/EchoCall.h \
    include/utils/ScopedGuard.h \
    include/voip/Call.h \
    include/voip/CallAnswer.h \
    include/voip/CallConnectionFactory.h \
    include/voip/CallEnded.h \
    include/voip/CallError.h \
    include/voip/CallException.h \
    include/voip/CallManager.h \
    include/voip/CallOffer.h \
    include/voip/CallReceived.h \
    include/voip/CallRejected.h \
    include/voip/CallSignalingMessage.h \
    include/voip/IceCandidate.h \
    include/voip/IncomingCall.h \
    include/voip/OutgoingCall.h \
    include/voip/PlatformAudio.h \
    include/voip/PlatformCallManager.h \
    include/voip/PlatformError.h \
    include/voip/PlatformException.h \
    src/queue/dispatch_queue.h \
    src/voip/Observers.h \
    src/voip/PeerConnectionQueue.h \
    src/voip/WebRtcResource.h

SOURCES += \
    src/EchoCall.cpp \
    src/main.cpp \
    src/queue/dispatch_queue.cpp \
    src/voip/Call.cpp \
    src/voip/CallAnswer.cpp \
    src/voip/CallConnectionFactory.cpp \
    src/voip/CallEnded.cpp \
    src/voip/CallException.cpp \
    src/voip/CallManager.cpp \
    src/voip/CallOffer.cpp \
    src/voip/CallReceived.cpp \
    src/voip/CallRejected.cpp \
    src/voip/CallSignalingMessage.cpp \
    src/voip/IceCandidate.cpp \
    src/voip/IncomingCall.cpp \
    src/voip/Observers.cpp \
    src/voip/OutgoingCall.cpp \
    src/voip/PeerConnectionQueue.cpp \
    src/voip/PlatformException.cpp \
    src/voip/WebRtcResource.cpp

RESOURCES += \
    src/ui/qml.qrc

INCLUDEPATH += \
    include \
    src \
    include/utils \
    include/voip \
    src/voip

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 3rdparty
macx {
    message("* Using settings for MacOS.")
    WEBRTC_LIB_SUBDIR = "mac"
    LIBS += -L$$PWD/3rdparty/WebRTC/mac/lib -lwebrtc_d
    LIBS += -framework Foundation -framework CoreServices -framework ApplicationServices -framework CoreAudio -framework AudioToolbox
    DEFINES += WEBRTC_MAC WEBRTC_POSIX WEBRTC_UNIX
    HEADERS += \
        src/voip/PlatformAudioWebRtc.h \
        src/voip/PlatformCallManagerDefault.h
    SOURCES += \
        src/voip/PlatformAudioWebRtc.cpp \
        src/voip/PlatformCallManagerDefault.cpp
}

linux:!android {
    message("* Using settings for Linux.")
    WEBRTC_LIB_SUBDIR = "linux"
    LIBS += -L$$PWD/3rdparty/WebRTC/linux/lib/x86_64 -lwebrtc_d
    LIBS += -framework Foundation \
            -framework CoreServices \
            -framework ApplicationServices \
            -framework CoreAudio \
            -framework AudioToolbox
    DEFINES += WEBRTC_MAC WEBRTC_POSIX WEBRTC_UNIX
    HEADERS += \
        src/voip/PlatformAudioWebRtc.h \
        src/voip/PlatformCallManagerDefault.h
    SOURCES += \
        src/voip/PlatformAudioWebRtc.cpp \
        src/voip/PlatformCallManagerDefault.cpp
}

android {
    message("* Using settings for Android.")
    WEBRTC_LIB_SUBDIR = "android"
    LIBS += -L$$PWD/3rdparty/WebRTC/android/lib/$$ANDROID_TARGET_ARCH -lwebrtc_d
    LIBS += -landroid -lOpenSLES
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
    ANDROID_MIN_SDK_VERSION = 28
    ANDROID_TARGET_SDK_VERSION = 28
    ANDROID_API_VERSION = 9
    DEFINES += WEBRTC_LINUX WEBRTC_ANDROID WEBRTC_POSIX
    HEADERS += \
        src/voip/android/PlatformAudioAndroid.h \
        src/voip/PlatformCallManagerDefault.h
    SOURCES += \
        src/jni/init_android.cpp \
        src/voip/android/PlatformAudioAndroid.cpp \
        src/voip/PlatformCallManagerDefault.cpp
}

ios {
    message("* Using settings for iOS.")
    WEBRTC_LIB_SUBDIR = "ios"
    QMAKE_IOS_DEPLOYMENT_TARGET = 13.0
    QMAKE_INFO_PLIST = $$PWD/ios/Info.plist
    QMAKE_LFLAGS += -force_load $$PWD/3rdparty/WebRTC/ios/lib/libwebrtc_d.a
    QMAKE_OBJECTIVE_CFLAGS += -fobjc-arc
    LIBS += -framework Foundation \
            -framework CoreServices \
            -framework CoreAudio \
            -framework AudioToolbox \
            -framework AVFoundation \
            -framework CoreMedia \
            -framework CallKit \
            -framework Intents
    DEFINES += WEBRTC_MAC WEBRTC_IOS WEBRTC_POSIX WEBRTC_UNIX
    INCLUDEPATH += \
        $$PWD/3rdparty/WebRTC/ios/include/webrtc/sdk/objc/base
    HEADERS += \
        src/voip/ios/PlatformAudioIOS.h \
        src/voip/ios/PlatformCallManagerIOS.h
    OBJECTIVE_SOURCES += \
        ios/AppDelegate.mm \
        src/voip/ios/PlatformAudioIOS.mm \
        src/voip/ios/PlatformCallManagerIOS.mm
}

QMAKE_INCDIR += \
    $$PWD/3rdparty \
    $$PWD/3rdparty/WebRTC/$$WEBRTC_LIB_SUBDIR/include \
    $$PWD/3rdparty/WebRTC/$$WEBRTC_LIB_SUBDIR/include/webrtc \
    $$PWD/3rdparty/WebRTC/$$WEBRTC_LIB_SUBDIR/include/webrtc/third_party \
    $$PWD/3rdparty/WebRTC/$$WEBRTC_LIB_SUBDIR/include/webrtc/third_party/abseil-cpp
