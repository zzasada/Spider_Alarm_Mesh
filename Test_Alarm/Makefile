PROJECT_NAME = Test_Alarm

TARGET ?= nrf52840-os
# LIBDIR ?= $(CURDIR)/../../..
LIBDIR ?= f:/Mira/libmira-2.4.2/libmira

CFLAGS += -DNRF52840_XXAA
CFLAGS += -I $(CURDIR)
CFLAGS += -I $(LIBDIR)/nRF5SDK/modules/nrfx/mdk
CFLAGS += -I $(LIBDIR)/nRF5SDK/components/toolchain/cmsis/include
CFLAGS += -I $(CURDIR)/../Common
CFLAGS += -I $(CURDIR)/../Common/nrf_SDK_files

SOURCE_FILES = \
	pwm/pwm_lib.c \
	Main.c

include $(LIBDIR)/Makefile.include
