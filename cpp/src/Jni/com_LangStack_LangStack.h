

#ifndef _INCLUDE_LANGUAGE_STACK_LSTACK_JNI_H_
#define _INCLUDE_LANGUAGE_STACK_LSTACK_JNI_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>
#include "Defs.h"

////////////////////////////////////////////////////////////////////////////////
/// for LangStack
LS_EXPORT void JNICALL Java_com_LangStack_LangStack_startJniMode
  (JNIEnv *, jclass);

LS_EXPORT void JNICALL Java_com_LangStack_LangStack_startTcpMode
  (JNIEnv *, jclass, jint);

////////////////////////////////////////////////////////////////////////////////
/// for Log
LS_EXPORT void JNICALL Java_com_LangStack_Log_Logger_log
  (JNIEnv *, jclass, jint, jstring, jstring);
LS_EXPORT void JNICALL Java_com_LangStack_Log_Logger_setLoggerLevel
  (JNIEnv *, jclass, jint);

////////////////////////////////////////////////////////////////////////////////
/// for Transport java Jnisession
LS_EXPORT void JNICALL Java_com_LangStack_Transport_sendCall2Cpp
  (JNIEnv *, jclass, jstring);

LS_EXPORT void JNICALL Java_com_LangStack_Transport_sendReturn2Cpp
  (JNIEnv *, jclass, jstring);

////////////////////////////////////////////////////////////////////////////////
/// for Transport cpp Jnisession
LS_EXPORT void sendCall2Java(const char *str);
LS_EXPORT void sendReturn2Java(const char *str);

////////////////////////////////////////////////////////////////////////////////
/// for jni
LS_EXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved);

LS_EXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved);

#ifdef __cplusplus
}
#endif
#endif
