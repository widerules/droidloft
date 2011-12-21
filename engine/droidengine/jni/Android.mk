LOCAL_PATH := $(call my-dir)

###########################################################
## Find all of files under the named directories.
###########################################################

define all-cpp-files-under
$(patsubst ./%,%, \
  $(shell cd $(LOCAL_PATH) ; \
          find $(1) -name "*.cpp" -and -not -name ".*") \
 )
endef

#################################################################################
# build wienginedemos lib
#################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := gdx
GDX_CFLAGS 	  := -O2 -Wall -D__ANDROID__
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/../../lib/jniso/$(TARGET_ARCH_ABI) -llog -lGLESv1_CM -lwiengine -lchipmunk -llua -lwisound -lbox2d -lcurl
#LOCAL_LDLIBS := -L/cygdrive/v/src/prj/lib/jniso/armeabi -llog -lGLESv1_CM -lwiengine -lchipmunk -llua -lwisound -lbox2d -lcurl

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Box2D \
	$(LOCAL_PATH)/Box2D/Collision

LOCAL_CFLAGS    := $(GDX_CFLAGS)
LOCAL_SRC_FILES := $(call all-cpp-files-under,Box2D)
	
include $(BUILD_SHARED_LIBRARY)
