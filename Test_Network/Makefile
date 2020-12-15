PROJECT_NAME = network_sender

TARGET ?= nrf52840ble-os
LIBDIR ?= f:/Mira/libmira-2.4.2/libmira

CFLAGS += -DNRF52840_XXAA
CFLAGS += -I $(CURDIR)
CFLAGS += -I $(LIBDIR)/nRF5SDK/modules/nrfx/mdk
CFLAGS += -I $(LIBDIR)/nRF5SDK/components/toolchain/cmsis/include
CFLAGS += -I $(CURDIR)/../Common
CFLAGS += -I $(CURDIR)/../Common/nrf_SDK_files

SOURCE_FILES = \
	test_network_sender.c

include $(LIBDIR)/Makefile.include
