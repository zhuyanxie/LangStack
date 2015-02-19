/******************************************************************************
Copyright (C) 2015 zhuyanxie

　　Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

　　The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "com_langstack_LangStack.h"

#include "LangStack.h"
#include "Log/Log.h"
#include "Transport/JniSession.h"

JavaVM *g_jvm = 0;

////////////////////////////////////////////////////////////////////////////////
/// for LangStack
JNIEXPORT void JNICALL Java_com_LangStack_LangStack_startJniMode
  (JNIEnv *env, jclass cls)
{
    ls::CLangStack::startJniMode();
}

JNIEXPORT void JNICALL Java_com_LangStack_LangStack_startTcpMode
  (JNIEnv *env, jclass cls, jint port)
{
    ls::CLangStack::startTcpMode((uint16_t)port);
}
////////////////////////////////////////////////////////////////////////////////
/// for Log
JNIEXPORT void JNICALL Java_com_LangStack_Log_Logger_log
  (JNIEnv *env, jclass cls, jint lev, jstring tag, jstring detail)
{
	const char* ctag = env->GetStringUTFChars(tag, nullptr);
	const char* cdetail = env->GetStringUTFChars(detail, nullptr);
	langstackLogPrintFull(LogLevel(lev), "java", 0, ctag, "%s", cdetail);
	env->ReleaseStringUTFChars(tag, ctag);
	env->ReleaseStringUTFChars(detail, cdetail);
}

////////////////////////////////////////////////////////////////////////////////
/// for Transport java Jnisession
JNIEXPORT void JNICALL Java_com_LangStack_Transport_sendCall2Cpp
  (JNIEnv *env, jclass cls, jstring str)
{
	const char* buf = env->GetStringUTFChars(str, nullptr);
	ls::CJniSession::onJavaCall(buf);
	env->ReleaseStringUTFChars(str, buf);
}

JNIEXPORT void JNICALL Java_com_LangStack_Transport_sendReturn2Cpp
 (JNIEnv *env, jclass cls, jstring str)
{
	const char* buf = env->GetStringUTFChars(str, nullptr);
	ls::CJniSession::onJavaReturn(buf);
	env->ReleaseStringUTFChars(str, buf);
}

////////////////////////////////////////////////////////////////////////////////
/// for Transport cpp Jnisession
void sendCall2Java(const char *str)
{
	JNIEnv *env = nullptr;
    g_jvm->AttachCurrentThread(&env, nullptr);
    do
    {
		if (env == nullptr)
		{
			break;
		}
		jstring jstr = env->NewStringUTF(str);
		jclass cls = env->FindClass("com/LangStack/Transport/JniSession");
		if (cls == nullptr)
		{
			break;
		}
		jmethodID mid = env->GetStaticMethodID(cls, "onCppCall", "(Ljava/lang/String;)V");
		if (mid == nullptr)
		{
			break;
		}
		env->CallStaticVoidMethod(cls, mid, jstr);
    } while(0);
	g_jvm->DetachCurrentThread();
}

void sendReturn2Java(const char *str)
{
	JNIEnv *env = nullptr;
    g_jvm->AttachCurrentThread(&env, nullptr);
    do
    {
		if (env == nullptr)
		{
			break;
		}
		jstring jstr = env->NewStringUTF(str);
		jclass cls = env->FindClass("com/LangStack/Transport/JniSession");
		if (cls == nullptr)
		{
			break;
		}
		jmethodID mid = env->GetStaticMethodID(cls, "onCppReturn", "(Ljava/lang/String;)V");
		if (mid == nullptr)
		{
			break;
		}
		env->CallStaticVoidMethod(cls, mid, jstr);
    } while(0);
	g_jvm->DetachCurrentThread();
}

////////////////////////////////////////////////////////////////////////////////
/// for jni
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    g_jvm = vm;
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved)
{

}
