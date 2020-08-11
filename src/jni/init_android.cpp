#include <jni.h>

#include <webrtc/modules/utility/include/jvm_android.h>
#include <webrtc/sdk/android/native_api/base/init.h>


extern "C" jint JNIEXPORT JNICALL
JNI_OnLoad(JavaVM *jvm, void *) {
    webrtc::InitAndroid(jvm);
    webrtc::JVM::Initialize(jvm);
    return JNI_VERSION_1_6;
}
