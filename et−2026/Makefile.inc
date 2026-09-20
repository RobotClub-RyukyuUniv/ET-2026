mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

# オブジェクトファイルを追加 (.o)
APPL_CXXOBJS += \
	Tracer.o \
	ColorSensorManager.o \
	DualMotorController.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-spike/Makefile

endif

# ディレクトリの指定
APPL_DIRS += $(mkfile_path)app $(mkfile_path)unit $(mkfile_path)device

# インクルードパスの指定（末尾のバックスラッシュ \ に注意）
INCLUDES += \
	-I$(mkfile_path)../../../drivers/include \
	-I$(mkfile_path)../../../drivers/include/spike \
	-I$(mkfile_path)../../../drivers/include/libcpp/spike \
	-I$(mkfile_path)app \
	-I$(mkfile_path)device