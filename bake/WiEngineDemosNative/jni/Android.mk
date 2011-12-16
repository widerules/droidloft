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
LOCAL_MODULE := wienginedemos
LOCAL_LDLIBS := -L$(LOCAL_PATH)/../../lib/jniso/$(TARGET_ARCH_ABI) -llog -lGLESv1_CM -lwiengine -lchipmunk -llua -lwisound -lbox2d -lcurl
#LOCAL_LDLIBS := -L/cygdrive/v/src/prj/lib/jniso/armeabi -llog -lGLESv1_CM -lwiengine -lchipmunk -llua -lwisound -lbox2d -lcurl

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Demos \
	$(LOCAL_PATH)/../../lib/jnihead \
	$(LOCAL_PATH)/../../lib/jnihead/chipmunk \
	$(LOCAL_PATH)/../../lib/jnihead/chipmunk/constraints \
	$(LOCAL_PATH)/../../lib/jnihead/libcurl/include \
	$(LOCAL_PATH)/../../lib/jnihead/lua \
	$(LOCAL_PATH)/../../lib/jnihead/lua/tolua++ \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/actions \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/afcanim \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/astar \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/bitmapfont \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/box2d \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/chipmunk \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/common \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/easeactions \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/events \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/filters \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/gridactions \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/grid \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/lua \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/mfs \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/network \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/nodes \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/opengl \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/particle \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/persistence \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/tmx \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/transitions \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/types \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/wisound \
	$(LOCAL_PATH)/../../lib/jnihead/WiEngine/include/utils \
	$(LOCAL_PATH)/../../lib/jnihead/libxml2/include
LOCAL_SRC_FILES := $(call all-cpp-files-under,Demos)
	
include $(BUILD_SHARED_LIBRARY)
