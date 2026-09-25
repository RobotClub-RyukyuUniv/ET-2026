mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_CXXOBJS += \
	Tracer.o \
	ArmManager.o \
	ColorSensorManager.o \
	DualMotorController.o \
	HeadingSensorManager.o \
	TimerManager.o \
	UltrasonicDistanceSensorManager.o \
	StraightRunner.o \
	Rotator.o \
	LineTracer.o \
	Odometry.o \
	MedianFilter.o \
	BottleExtractor.o \
	BottleSearcher.o \
	BottlePusher.o \
	ReturnAction.o \
	ButtonManager.o \
	CourseRunManager.o \
	LapTraceState.o \
	PlaceBottleState.o \
	PrepareResumeState.o \
	TraceToDestinationState.o \
	MeasureBottleColorState.o \
	PrepareMeasureState.o \
	CourseReturnState.o \
	TraceToSecondBlueState.o \
	ObjectCenterAligner.o \
	EmaFilter.o \
	ApproachCircleState.o \
	EtSumoExecuteState.o \
	EtSumoReturnState.o \
	ToEtSumoRouteState.o \
	GoalRunState.o \
	FinishedState.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-spike/Makefile

endif

# ディレクトリの指定
APPL_DIRS += \
	$(mkfile_path)app \
	$(mkfile_path)device \
	$(mkfile_path)controllers \
	$(mkfile_path)calculation \
	$(mkfile_path)manager \
	$(mkfile_path)manager/states \

# インクルードパスの指定
INCLUDES += \
	-I$(mkfile_path)../../../drivers/include \
	-I$(mkfile_path)../../../drivers/include/spike \
	-I$(mkfile_path)../../../drivers/include/libcpp/spike \
	-I$(mkfile_path)app \
	-I$(mkfile_path)device \
	-I$(mkfile_path)controllers \
	-I$(mkfile_path)calculation \
	-I$(mkfile_path)manager \
	-I$(mkfile_path)manager/states \
