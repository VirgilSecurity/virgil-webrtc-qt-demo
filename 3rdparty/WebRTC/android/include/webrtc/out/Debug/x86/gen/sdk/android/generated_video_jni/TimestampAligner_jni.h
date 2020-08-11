// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


// This file is autogenerated by
//     base/android/jni_generator/jni_generator.py
// For
//     org/webrtc/TimestampAligner

#ifndef org_webrtc_TimestampAligner_JNI
#define org_webrtc_TimestampAligner_JNI

#include <jni.h>

#include "../../../../../../../sdk/android/src/jni/jni_generator_helper.h"


// Step 1: Forward declarations.

JNI_REGISTRATION_EXPORT extern const char kClassPath_org_webrtc_TimestampAligner[];
const char kClassPath_org_webrtc_TimestampAligner[] = "org/webrtc/TimestampAligner";
// Leaking this jclass as we cannot use LazyInstance from some threads.
JNI_REGISTRATION_EXPORT std::atomic<jclass> g_org_webrtc_TimestampAligner_clazz(nullptr);
#ifndef org_webrtc_TimestampAligner_clazz_defined
#define org_webrtc_TimestampAligner_clazz_defined
inline jclass org_webrtc_TimestampAligner_clazz(JNIEnv* env) {
  return base::android::LazyGetClass(env, kClassPath_org_webrtc_TimestampAligner,
      &g_org_webrtc_TimestampAligner_clazz);
}
#endif


// Step 2: Constants (optional).


// Step 3: Method stubs.
namespace  webrtc {
namespace jni {

static jlong JNI_TimestampAligner_RtcTimeNanos(JNIEnv* env);

JNI_GENERATOR_EXPORT jlong Java_org_webrtc_TimestampAligner_nativeRtcTimeNanos(
    JNIEnv* env,
    jclass jcaller) {
  return JNI_TimestampAligner_RtcTimeNanos(env);
}

static jlong JNI_TimestampAligner_CreateTimestampAligner(JNIEnv* env);

JNI_GENERATOR_EXPORT jlong Java_org_webrtc_TimestampAligner_nativeCreateTimestampAligner(
    JNIEnv* env,
    jclass jcaller) {
  return JNI_TimestampAligner_CreateTimestampAligner(env);
}

static void JNI_TimestampAligner_ReleaseTimestampAligner(JNIEnv* env, jlong timestampAligner);

JNI_GENERATOR_EXPORT void Java_org_webrtc_TimestampAligner_nativeReleaseTimestampAligner(
    JNIEnv* env,
    jclass jcaller,
    jlong timestampAligner) {
  return JNI_TimestampAligner_ReleaseTimestampAligner(env, timestampAligner);
}

static jlong JNI_TimestampAligner_TranslateTimestamp(JNIEnv* env, jlong timestampAligner,
    jlong cameraTimeNs);

JNI_GENERATOR_EXPORT jlong Java_org_webrtc_TimestampAligner_nativeTranslateTimestamp(
    JNIEnv* env,
    jclass jcaller,
    jlong timestampAligner,
    jlong cameraTimeNs) {
  return JNI_TimestampAligner_TranslateTimestamp(env, timestampAligner, cameraTimeNs);
}


}  // namespace jni
}  // namespace  webrtc

// Step 4: Generated test functions (optional).


#endif  // org_webrtc_TimestampAligner_JNI
