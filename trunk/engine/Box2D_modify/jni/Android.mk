LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := helloworld

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := HelloWorld.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH) \
		    $(LOCAL_PATH)/Box2D \
		    $(LOCAL_PATH)/Collision \
		    $(LOCAL_PATH)/Common \
		    $(LOCAL_PATH)/Dynamics \
		    $(LOCAL_PATH)/Rope

#LOCAL_LDIBS := -lstdc++

LOCAL_STATIC_LIBRARIES := libbox2d

include $(BUILD_EXECUTABLE)
##################################
include $(call all-makefiles-under,$(LOCAL_PATH))
