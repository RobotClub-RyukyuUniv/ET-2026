mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

# オブジェクトファイルを追加 (.o)
APPL_CXXOBJS += \
	Tracer.o \
	LineTracer.o \
	ColorSensorManager.o \
	DualMotorController.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-spike/Makefile

endif

# ディレクトリの指定
APPL_DIRS += \
	$(mkfile_path)app \
	$(mkfile_path)unit \
	$(mkfile_path)device \
	$(mkfile_path)tracer

# インクルードパスの指定
INCLUDES += \
	-I$(mkfile_path)../../../drivers/include \
	-I$(mkfile_path)../../../drivers/include/spike \
	-I$(mkfile_path)../../../drivers/include/libcpp/spike \
	-I$(mkfile_path)app \
	-I$(mkfile_path)device \
	-I$(mkfile_path)tracer