# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# JNI
#
LOCAL_PATH := $(call my-dir)
CPP_SRC_PATH := ../../../cpp
TEST_SRC_PATH := ../..

include $(CLEAR_VARS)
LOCAL_MODULE    := testLangStack

#头文件路径声明
LOCAL_C_INCLUDES += $(TEST_SRC_PATH)
LOCAL_C_INCLUDES += $(TEST_SRC_PATH)/gtest
LOCAL_C_INCLUDES += $(TEST_SRC_PATH)/src
LOCAL_C_INCLUDES += $(CPP_SRC_PATH)/include

#源文件路径声明
# CPP_DIRS := $(shell find $(TEST_SRC_PATH)/src -maxdepth 2 -type d)
CPP_DIRS := ../../src

#查找cpp源文件
LOCAL_SRC_FILES := $(foreach dir, ${CPP_DIRS}, $(wildcard $(dir)/*.cpp))

LOCAL_LDFLAGS   += -L$(CPP_SRC_PATH)/libs/armeabi

LOCAL_LDLIBS	:= -L$(SYSROOT)/usr/lib -llog 
LOCAL_LDLIBS	+= -lLangStack

LOCAL_SHARED_LIBRARIES := -lstdc++ -lstlport

LOCAL_STATIC_LIBRARIES += libc

LOCAL_CPPFLAGS	:= -std=c++11 -pthread -fexceptions -frtti -Dandroid
LOCAL_CPPFLAGS	+= -g -Wall
LOCAL_CFLAGS	+= -g -Wall

include $(BUILD_EXECUTABLE)
