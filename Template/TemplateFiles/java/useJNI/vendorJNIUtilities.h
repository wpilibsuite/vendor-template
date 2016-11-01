#pragma once

#include <jni.h>
#include <stdint.h>

namespace $vendorreplace$ {

void ReportError(JNIEnv *env, int32_t status, bool do_throw);

}

inline bool CheckStatus(JNIEnv *env, int32_t status, bool do_throw = true) {
  if (status != 0) $vendorreplace$::ReportError(env, status, do_throw);
  return status == 0;
}
