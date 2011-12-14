LOCAL_PATH := $(call my-dir)

###########################################################
## Find all of files under the named directories.
###########################################################

define all-c-files-under
$(patsubst ./%,%, \
  $(shell cd $(LOCAL_PATH) ; \
          find $(1) -name "*.c" -and -not -name ".*") \
 )
endef

#################################################################################
# build hijni lib
#################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := hijni
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/../../WiEngine_library/libs/$(TARGET_ARCH_ABI) -llog -lGLESv1_CM -lwiengine -lchipmunk -llua -lwisound -lbox2d -lcurl
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/Demos \
#	$(LOCAL_PATH)/../../WiEngine_library/jni \
#	$(LOCAL_PATH)/../../WiEngine_library/jni/chipmunk 

#LOCAL_SRC_FILES := $(call all-c-files-under,hijni)
LOCAL_SRC_FILES := hijni.c
	
include $(BUILD_SHARED_LIBRARY)
