// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


// This file is autogenerated by
//     base/android/jni_generator/jni_generator.py
// For
//     org/webrtc/VideoDecoder

#ifndef org_webrtc_VideoDecoder_JNI
#define org_webrtc_VideoDecoder_JNI

#include <jni.h>

#include "../../../../../../../sdk/android/src/jni/jni_generator_helper.h"


// Step 1: Forward declarations.

JNI_REGISTRATION_EXPORT extern const char kClassPath_org_webrtc_VideoDecoder[];
const char kClassPath_org_webrtc_VideoDecoder[] = "org/webrtc/VideoDecoder";

JNI_REGISTRATION_EXPORT extern const char kClassPath_org_webrtc_VideoDecoder_00024Settings[];
const char kClassPath_org_webrtc_VideoDecoder_00024Settings[] = "org/webrtc/VideoDecoder$Settings";
// Leaking this jclass as we cannot use LazyInstance from some threads.
JNI_REGISTRATION_EXPORT std::atomic<jclass> g_org_webrtc_VideoDecoder_clazz(nullptr);
#ifndef org_webrtc_VideoDecoder_clazz_defined
#define org_webrtc_VideoDecoder_clazz_defined
inline jclass org_webrtc_VideoDecoder_clazz(JNIEnv* env) {
  return base::android::LazyGetClass(env, kClassPath_org_webrtc_VideoDecoder,
      &g_org_webrtc_VideoDecoder_clazz);
}
#endif
// Leaking this jclass as we cannot use LazyInstance from some threads.
JNI_REGISTRATION_EXPORT std::atomic<jclass> g_org_webrtc_VideoDecoder_00024Settings_clazz(nullptr);
#ifndef org_webrtc_VideoDecoder_00024Settings_clazz_defined
#define org_webrtc_VideoDecoder_00024Settings_clazz_defined
inline jclass org_webrtc_VideoDecoder_00024Settings_clazz(JNIEnv* env) {
  return base::android::LazyGetClass(env, kClassPath_org_webrtc_VideoDecoder_00024Settings,
      &g_org_webrtc_VideoDecoder_00024Settings_clazz);
}
#endif


// Step 2: Constants (optional).


// Step 3: Method stubs.
namespace  webrtc {
namespace jni {


static std::atomic<jmethodID> g_org_webrtc_VideoDecoder_00024Settings_Constructor(nullptr);
static base::android::ScopedJavaLocalRef<jobject> Java_Settings_Constructor(JNIEnv* env,
    JniIntWrapper numberOfCores,
    JniIntWrapper width,
    JniIntWrapper height) {
  jclass clazz = org_webrtc_VideoDecoder_00024Settings_clazz(env);
  CHECK_CLAZZ(env, clazz,
      org_webrtc_VideoDecoder_00024Settings_clazz(env), NULL);

  jni_generator::JniJavaCallContextChecked call_context;
  call_context.Init<
      base::android::MethodID::TYPE_INSTANCE>(
          env,
          clazz,
          "<init>",
          "(III)V",
          &g_org_webrtc_VideoDecoder_00024Settings_Constructor);

  jobject ret =
      env->NewObject(clazz,
          call_context.base.method_id, as_jint(numberOfCores), as_jint(width), as_jint(height));
  return base::android::ScopedJavaLocalRef<jobject>(env, ret);
}

static std::atomic<jmethodID> g_org_webrtc_VideoDecoder_createNativeVideoDecoder(nullptr);
static jlong Java_VideoDecoder_createNativeVideoDecoder(JNIEnv* env, const
    base::android::JavaRef<jobject>& obj) {
  jclass clazz = org_webrtc_VideoDecoder_clazz(env);
  CHECK_CLAZZ(env, obj.obj(),
      org_webrtc_VideoDecoder_clazz(env), 0);

  jni_generator::JniJavaCallContextChecked call_context;
  call_context.Init<
      base::android::MethodID::TYPE_INSTANCE>(
          env,
          clazz,
          "createNativeVideoDecoder",
          "()J",
          &g_org_webrtc_VideoDecoder_createNativeVideoDecoder);

  jlong ret =
      env->CallLongMethod(obj.obj(),
          call_context.base.method_id);
  return ret;
}

static std::atomic<jmethodID> g_org_webrtc_VideoDecoder_initDecode(nullptr);
static base::android::ScopedJavaLocalRef<jobject> Java_VideoDecoder_initDecode(JNIEnv* env, const
    base::android::JavaRef<jobject>& obj, const base::android::JavaRef<jobject>& settings,
    const base::android::JavaRef<jobject>& decodeCallback) {
  jclass clazz = org_webrtc_VideoDecoder_clazz(env);
  CHECK_CLAZZ(env, obj.obj(),
      org_webrtc_VideoDecoder_clazz(env), NULL);

  jni_generator::JniJavaCallContextChecked call_context;
  call_context.Init<
      base::android::MethodID::TYPE_INSTANCE>(
          env,
          clazz,
          "initDecode",
"(Lorg/webrtc/VideoDecoder$Settings;Lorg/webrtc/VideoDecoder$Callback;)Lorg/webrtc/VideoCodecStatus;",
          &g_org_webrtc_VideoDecoder_initDecode);

  jobject ret =
      env->CallObjectMethod(obj.obj(),
          call_context.base.method_id, settings.obj(), decodeCallback.obj());
  return base::android::ScopedJavaLocalRef<jobject>(env, ret);
}

static std::atomic<jmethodID> g_org_webrtc_VideoDecoder_release(nullptr);
static base::android::ScopedJavaLocalRef<jobject> Java_VideoDecoder_release(JNIEnv* env, const
    base::android::JavaRef<jobject>& obj) {
  jclass clazz = org_webrtc_VideoDecoder_clazz(env);
  CHECK_CLAZZ(env, obj.obj(),
      org_webrtc_VideoDecoder_clazz(env), NULL);

  jni_generator::JniJavaCallContextChecked call_context;
  call_context.Init<
      base::android::MethodID::TYPE_INSTANCE>(
          env,
          clazz,
          "release",
          "()Lorg/webrtc/VideoCodecStatus;",
          &g_org_webrtc_VideoDecoder_release);

  jobject ret =
      env->CallObjectMethod(obj.obj(),
          call_context.base.method_id);
  return base::android::ScopedJavaLocalRef<jobject>(env, ret);
}

static std::atomic<jmethodID> g_org_webrtc_VideoDecoder_decode(nullptr);
static base::android::ScopedJavaLocalRef<jobject> Java_VideoDecoder_decode(JNIEnv* env, const
    base::android::JavaRef<jobject>& obj, const base::android::JavaRef<jobject>& frame,
    const base::android::JavaRef<jobject>& info) {
  jclass clazz = org_webrtc_VideoDecoder_clazz(env);
  CHECK_CLAZZ(env, obj.obj(),
      org_webrtc_VideoDecoder_clazz(env), NULL);

  jni_generator::JniJavaCallContextChecked call_context;
  call_context.Init<
      base::android::MethodID::TYPE_INSTANCE>(
          env,
          clazz,
          "decode",
"(Lorg/webrtc/EncodedImage;Lorg/webrtc/VideoDecoder$DecodeInfo;)Lorg/webrtc/VideoCodecStatus;",
          &g_org_webrtc_VideoDecoder_decode);

  jobject ret =
      env->CallObjectMethod(obj.obj(),
          call_context.base.method_id, frame.obj(), info.obj());
  return base::android::ScopedJavaLocalRef<jobject>(env, ret);
}

static std::atomic<jmethodID> g_org_webrtc_VideoDecoder_getPrefersLateDecoding(nullptr);
static jboolean Java_VideoDecoder_getPrefersLateDecoding(JNIEnv* env, const
    base::android::JavaRef<jobject>& obj) {
  jclass clazz = org_webrtc_VideoDecoder_clazz(env);
  CHECK_CLAZZ(env, obj.obj(),
      org_webrtc_VideoDecoder_clazz(env), false);

  jni_generator::JniJavaCallContextChecked call_context;
  call_context.Init<
      base::android::MethodID::TYPE_INSTANCE>(
          env,
          clazz,
          "getPrefersLateDecoding",
          "()Z",
          &g_org_webrtc_VideoDecoder_getPrefersLateDecoding);

  jboolean ret =
      env->CallBooleanMethod(obj.obj(),
          call_context.base.method_id);
  return ret;
}

static std::atomic<jmethodID> g_org_webrtc_VideoDecoder_getImplementationName(nullptr);
static base::android::ScopedJavaLocalRef<jstring> Java_VideoDecoder_getImplementationName(JNIEnv*
    env, const base::android::JavaRef<jobject>& obj) {
  jclass clazz = org_webrtc_VideoDecoder_clazz(env);
  CHECK_CLAZZ(env, obj.obj(),
      org_webrtc_VideoDecoder_clazz(env), NULL);

  jni_generator::JniJavaCallContextChecked call_context;
  call_context.Init<
      base::android::MethodID::TYPE_INSTANCE>(
          env,
          clazz,
          "getImplementationName",
          "()Ljava/lang/String;",
          &g_org_webrtc_VideoDecoder_getImplementationName);

  jstring ret =
      static_cast<jstring>(env->CallObjectMethod(obj.obj(),
          call_context.base.method_id));
  return base::android::ScopedJavaLocalRef<jstring>(env, ret);
}

}  // namespace jni
}  // namespace  webrtc

// Step 4: Generated test functions (optional).


#endif  // org_webrtc_VideoDecoder_JNI