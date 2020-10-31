PROJECT_NAME = Spider_Alarm

TARGET ?= nrf52840ble-os
LIBDIR ?= $(CURDIR)/../../..

CFLAGS += -DNRF52840_XXAA
CFLAGS += -I $(CURDIR)
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/modules/nrfx/mdk
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/toolchain/cmsis/include
CFLAGS += -I $(CURDIR)/../Common
CFLAGS += -I $(CURDIR)/../Common/nrf_SDK_files

SOURCE_FILES = \
	ak9756/ak9756.c \
	alarm/alarm.c \
	battery_monitor/battery.c \
	ble/ble.c \
	fota/fota.c \
	heartbeat/heartbeat.c \
	lis2dh/lis2dh.c \
	network/network.c \
	pwm/pwm_lib.c \
	rgb_led/rgb.c \
	config/app-config.c \
	nfc/net-status.c \
	nfc/nfc-if.c \
	app/application.c \
	../Common/common.c \
	self_test/self_test.c \
	boot.c 


include $(LIBDIR)/Makefile.include
