#include "$packageunderscores$_ReversibleDigitalInputJNI.h"
#include "$vendorreplace$JNIUtilities.h"
#include "ReversibleDigitalInputDriver.h"

extern "C" {
/*
 * Class:     com_wpilib_ReversibleDigitalInputJNI
 * Method:    InitializeReversibleDigitalInput
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_$packageunderscores$_ReversibleDigitalInputJNI_InitializeReversibleDigitalInput
  (JNIEnv * env, jclass, jint channel) {
  int32_t status = 0;
  auto handle = $vendorallcaps$_InitializeReversibleDigitalInput(channel, &status);
  CheckStatus(env, status);
  return handle;
}

/*
 * Class:     $packageunderscores$_ReversibleDigitalInputJNI
 * Method:    FreeReversibleDigitalInput
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_$packageunderscores$_ReversibleDigitalInputJNI_FreeReversibleDigitalInput
  (JNIEnv * env, jclass, jint handle) {
  $vendorallcaps$_FreeReversibleDigitalInput(handle);
}

/*
 * Class:     $packageunderscores$_ReversibleDigitalInputJNI
 * Method:    SetReversal
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_$packageunderscores$_ReversibleDigitalInputJNI_SetReversal
  (JNIEnv * env, jclass, jint handle, jboolean reverse) {
  int32_t status = 0;
  $vendorallcaps$_SetReverseReversibleDigitalInput(handle, reverse, &status);
  CheckStatus(env, status);
}

/*
 * Class:     $packageunderscores$_ReversibleDigitalInputJNI
 * Method:    GetReversibleDigitalInput
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_$packageunderscores$_ReversibleDigitalInputJNI_GetReversibleDigitalInput
  (JNIEnv * env, jclass, jint handle) 
  {
  int32_t status = 0;
  auto retVal = $vendorallcaps$_GetReversibleDigitalInputValue(handle, &status);
  CheckStatus(env, status);
  return retVal;
}
}
