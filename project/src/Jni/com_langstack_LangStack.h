
#include <jni.h>

#ifndef _INCLUDE_LANGUAGE_STACK_LSTACK_JNI_H_
#define _INCLUDE_LANGUAGE_STACK_LSTACK_JNI_H_
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_langstack_LangStack_startJniMode
  (JNIEnv *, jclass);

JNIEXPORT void JNICALL Java_com_langstack_LangStack_startTcpMode
  (JNIEnv *, jclass, jint);

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved);

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved);

#ifdef __cplusplus
}
#endif
#endif
