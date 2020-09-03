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
	pwm/pwm_lib.c \
	Main.c

include $(LIBDIR)/Makefile.include
