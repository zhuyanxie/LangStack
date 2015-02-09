
#include <jni.h>

#ifndef _INCLUDE_LANGUAGE_STACK_LSTACK_JNI_H_
#define _INCLUDE_LANGUAGE_STACK_LSTACK_JNI_H_
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_mm_Component_Event_LangStack
 * Method:    startCppServer
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_langstack_LangStack_startJniServer
  (JNIEnv *, jclass);

/*
 * Class:     com_mm_Component_Event_LangStack
 * Method:    startCppServer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_langstack_LangStack_startTcpServer
  (JNIEnv *, jclass, jint);

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved);

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved);

#ifdef __cplusplus
}
#endif
#endif
