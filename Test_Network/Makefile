PROJECT_NAME = network_sender

TARGET ?= nrf52840-os
LIBDIR ?= $(CURDIR)/../../..

CFLAGS += -DNRF52840_XXAA
CFLAGS += -I $(CURDIR)
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/modules/nrfx/mdk
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/toolchain/cmsis/include
CFLAGS += -I $(CURDIR)/../Common
CFLAGS += -I $(CURDIR)/../Common/nrf_SDK_files

SOURCE_FILES = \
	test_network_sender.c

include $(LIBDIR)/Makefile.include
