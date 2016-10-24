#include <jni.h>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <mutex>
#include <sstream>
#include <queue>
#include <thread>
#include <cstring>
#include <string>

#include "support/jni_util.h"
#include "HAL/HAL.h"

#include "$packageunderscores$_$implreplace$JNI.h"

#include "SampleDriver.h"


using namespace wpi::java;

// Used for callback.
static JavaVM *jvm = nullptr;
// Thread-attached environment for listener callbacks.

extern "C" {
  
static jclass runtimeExCls = nullptr;
static jclass throwableCls = nullptr;
static jclass stackTraceElementCls = nullptr;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
  jvm = vm;

  JNIEnv *env;
  if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK)
    return JNI_ERR;

  // Cache references to classes
  jclass local;
  
  local = env->FindClass("java/lang/Throwable");
  if (!local) return JNI_ERR;
  throwableCls = static_cast<jclass>(env->NewGlobalRef(local));
  if (!throwableCls) return JNI_ERR;
  env->DeleteLocalRef(local);

  local = env->FindClass("java/lang/StackTraceElement");
  if (!local) return JNI_ERR;
  stackTraceElementCls = static_cast<jclass>(env->NewGlobalRef(local));
  if (!stackTraceElementCls) return JNI_ERR;
  env->DeleteLocalRef(local);

  local = env->FindClass("java/lang/RuntimeException");
  if (!local) return JNI_ERR;
  runtimeExCls = static_cast<jclass>(env->NewGlobalRef(local));
  if (!runtimeExCls) return JNI_ERR;
  env->DeleteLocalRef(local);

  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
  JNIEnv *env;
  if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK)
    return;
  // Delete global references
  if (throwableCls) env->DeleteGlobalRef(throwableCls);
  if (stackTraceElementCls) env->DeleteGlobalRef(stackTraceElementCls);
  if (runtimeExCls) env->DeleteGlobalRef(runtimeExCls);
  jvm = nullptr;
}

}  // extern "C"


static void GetStackTrace(JNIEnv *env, std::string &res, std::string &func) {
  // create a throwable
  static jmethodID constructorId = nullptr;
  if (!constructorId)
    constructorId = env->GetMethodID(throwableCls, "<init>", "()V");
  jobject throwable = env->NewObject(throwableCls, constructorId);

  // retrieve information from the exception.
  // get method id
  // getStackTrace returns an array of StackTraceElement
  static jmethodID getStackTraceId = nullptr;
  if (!getStackTraceId)
    getStackTraceId = env->GetMethodID(throwableCls, "getStackTrace",
                                       "()[Ljava/lang/StackTraceElement;");

  // call getStackTrace
  jobjectArray stackTrace =
      static_cast<jobjectArray>(env->CallObjectMethod(throwable,
                                                      getStackTraceId));

  if (!stackTrace) return;

  // get length of the array
  jsize stackTraceLength = env->GetArrayLength(stackTrace);

  // get toString methodId of StackTraceElement class
  static jmethodID toStringId = nullptr;
  if (!toStringId)
    toStringId = env->GetMethodID(stackTraceElementCls, "toString",
                                  "()Ljava/lang/String;");

  bool haveLoc = false;
  for (jsize i = 0; i < stackTraceLength; i++) {
    // add the result of toString method of each element in the result
    jobject curStackTraceElement = env->GetObjectArrayElement(stackTrace, i);

    // call to string on the object
    jstring stackElementString =
        static_cast<jstring>(env->CallObjectMethod(curStackTraceElement,
                                                   toStringId));

    if (!stackElementString) {
      env->DeleteLocalRef(stackTrace);
      env->DeleteLocalRef(curStackTraceElement);
      return;
    }

    // add a line to res
    // res += " at ";
    const char *tmp = env->GetStringUTFChars(stackElementString, nullptr);
    res += tmp;
    res += '\n';

    // func is caller of immediate caller (if there was one)
    // or, if we see it, the first user function
    if (i == 1)
      func = tmp;
    else if (i > 1 && !haveLoc &&
             std::strncmp(tmp, "edu.wpi.first.wpilibj", 21) != 0) {
      func = tmp;
      haveLoc = true;
    }
    env->ReleaseStringUTFChars(stackElementString, tmp);

    env->DeleteLocalRef(curStackTraceElement);
    env->DeleteLocalRef(stackElementString);
  }

  // release java resources
  env->DeleteLocalRef(stackTrace);
}
  
static void ReportError(JNIEnv *env, int32_t status, bool do_throw) {
  if (status == 0) return;
  const char *message = HAL_GetErrorMessage(status);
  if (do_throw && status < 0) {
    char *buf = new char[strlen(message) + 30];
    sprintf(buf, " Code: %d. %s", status, message);
    env->ThrowNew(runtimeExCls, buf);
    delete[] buf;
  } else {
    std::string stack = " at ";
    std::string func;
    GetStackTrace(env, stack, func);
    HAL_SendError(1, status, 0, message, func.c_str(), stack.c_str(), 1);
  }
}
  

inline bool CheckStatus(JNIEnv *env, int32_t status, bool do_throw = true) {
  if (status != 0) ReportError(env, status, do_throw);
  return status == 0;
}

extern "C" {

/*
 * Class:     $packageunderscores$_$implreplace$JNI
 * Method:    writeStringToConsole
 * Signature: (Ljava/lang/String)I
 */
JNIEXPORT jint JNICALL Java_$packageunderscores$_$implreplace$JNI_writeStringToConsole
  (JNIEnv *env, jclass, jstring toWrite)
{
  return SAMPLE_WriteStringToConsole(JStringRef{env, toWrite}.c_str());
}
  
}  // extern "C"
