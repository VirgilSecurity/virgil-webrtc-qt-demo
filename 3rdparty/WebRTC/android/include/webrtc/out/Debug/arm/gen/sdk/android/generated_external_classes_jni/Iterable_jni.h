// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


// This file is autogenerated by
//     base/android/jni_generator/jni_generator.py
// For
//     java/lang/Iterable

#ifndef java_lang_Iterable_JNI
#define java_lang_Iterable_JNI

#include <jni.h>

#include "../../../../../../../sdk/android/src/jni/jni_generator_helper.h"


// Step 1: Forward declarations.

JNI_REGISTRATION_EXPORT extern const char kClassPath_java_lang_Iterable[];
const char kClassPath_java_lang_Iterable[] = "java/lang/Iterable";
// Leaking this jclass as we cannot use LazyInstance from some threads.
JNI_REGISTRATION_EXPORT std::atomic<jclass> g_java_lang_Iterable_clazz(nullptr);
#ifndef java_lang_Iterable_clazz_defined
#define java_lang_Iterable_clazz_defined
inline jclass java_lang_Iterable_clazz(JNIEnv* env) {
  return base::android::LazyGetClass(env, kClassPath_java_lang_Iterable,
      &g_java_lang_Iterable_clazz);
}
#endif


// Step 2: Constants (optional).


// Step 3: Method stubs.
namespace JNI_Iterable {


static std::atomic<jmethodID> g_java_lang_Iterable_iterator(nullptr);
static base::android::ScopedJavaLocalRef<jobject> Java_Iterable_iterator(JNIEnv* env, const
    base::android::JavaRef<jobject>& obj) __attribute__ ((unused));
static base::android::ScopedJavaLocalRef<jobject> Java_Iterable_iterator(JNIEnv* env, const
    base::android::JavaRef<jobject>& obj) {
  jclass clazz = java_lang_Iterable_clazz(env);
  CHECK_CLAZZ(env, obj.obj(),
      java_lang_Iterable_clazz(env), NULL);

  jni_generator::JniJavaCallContextChecked call_context;
  call_context.Init<
      base::android::MethodID::TYPE_INSTANCE>(
          env,
          clazz,
          "iterator",
          "()Ljava/util/Iterator;",
          &g_java_lang_Iterable_iterator);

  jobject ret =
      env->CallObjectMethod(obj.obj(),
          call_context.base.method_id);
  return base::android::ScopedJavaLocalRef<jobject>(env, ret);
}

static std::atomic<jmethodID> g_java_lang_Iterable_forEach(nullptr);
static void Java_Iterable_forEach(JNIEnv* env, const base::android::JavaRef<jobject>& obj, const
    base::android::JavaRef<jobject>& p0) __attribute__ ((unused));
static void Java_Iterable_forEach(JNIEnv* env, const base::android::JavaRef<jobject>& obj, const
    base::android::JavaRef<jobject>& p0) {
  jclass clazz = java_lang_Iterable_clazz(env);
  CHECK_CLAZZ(env, obj.obj(),
      java_lang_Iterable_clazz(env));

  jni_generator::JniJavaCallContextChecked call_context;
  call_context.Init<
      base::android::MethodID::TYPE_INSTANCE>(
          env,
          clazz,
          "forEach",
          "(Ljava/util/function/Consumer;)V",
          &g_java_lang_Iterable_forEach);

     env->CallVoidMethod(obj.obj(),
          call_context.base.method_id, p0.obj());
}

static std::atomic<jmethodID> g_java_lang_Iterable_spliterator(nullptr);
static base::android::ScopedJavaLocalRef<jobject> Java_Iterable_spliterator(JNIEnv* env, const
    base::android::JavaRef<jobject>& obj) __attribute__ ((unused));
static base::android::ScopedJavaLocalRef<jobject> Java_Iterable_spliterator(JNIEnv* env, const
    base::android::JavaRef<jobject>& obj) {
  jclass clazz = java_lang_Iterable_clazz(env);
  CHECK_CLAZZ(env, obj.obj(),
      java_lang_Iterable_clazz(env), NULL);

  jni_generator::JniJavaCallContextChecked call_context;
  call_context.Init<
      base::android::MethodID::TYPE_INSTANCE>(
          env,
          clazz,
          "spliterator",
          "()Ljava/util/Spliterator;",
          &g_java_lang_Iterable_spliterator);

  jobject ret =
      env->CallObjectMethod(obj.obj(),
          call_context.base.method_id);
  return base::android::ScopedJavaLocalRef<jobject>(env, ret);
}

}  // namespace JNI_Iterable

// Step 4: Generated test functions (optional).


#endif  // java_lang_Iterable_JNI
