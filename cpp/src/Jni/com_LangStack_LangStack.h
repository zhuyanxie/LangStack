
#include <jni.h>

#ifndef _INCLUDE_LANGUAGE_STACK_LSTACK_JNI_H_
#define _INCLUDE_LANGUAGE_STACK_LSTACK_JNI_H_
#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
/// for LangStack
JNIEXPORT void JNICALL Java_com_LangStack_LangStack_startJniMode
  (JNIEnv *, jclass);

JNIEXPORT void JNICALL Java_com_LangStack_LangStack_startTcpMode
  (JNIEnv *, jclass, jint);

////////////////////////////////////////////////////////////////////////////////
/// for Log
JNIEXPORT void JNICALL Java_com_LangStack_Log_Logger_log
  (JNIEnv *, jclass, jint, jstring, jstring);

////////////////////////////////////////////////////////////////////////////////
/// for Transport java Jnisession
JNIEXPORT void JNICALL Java_com_LangStack_Transport_sendCall2Cpp
  (JNIEnv *, jclass, jstring);

JNIEXPORT void JNICALL Java_com_LangStack_Transport_sendReturn2Cpp
  (JNIEnv *, jclass, jstring);

////////////////////////////////////////////////////////////////////////////////
/// for Transport cpp Jnisession
void sendCall2Java(const char *str);
void sendReturn2Java(const char *str);

////////////////////////////////////////////////////////////////////////////////
/// for jni
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved);

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved);

#ifdef __cplusplus
}
#endif
#endif
