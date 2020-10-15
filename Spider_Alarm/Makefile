PROJECT_NAME = Spider_Alarm

TARGET ?= nrf52840-os
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
	nfc/nfc.c \
	pwm/pwm_lib.c \
	rgb_led/rgb.c \
	Main.c

include $(LIBDIR)/Makefile.include
