PROJECT_NAME = fota_receiver

TARGET ?= nrf52840ble-os
LIBDIR ?= $(CURDIR)/../../..

CFLAGS += -I$(SDKDIR)/components/softdevice/s140/include/
CFLAGS += -I$(CURDIR)/..Common/s140
CFLAGS += -DNRF52840_XXAA
CFLAGS += -I $(CURDIR)
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/modules/nrfx/mdk
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/toolchain/cmsis/include
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/config/nrf52840/config
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/util
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/softdevice/s140/headers
# CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/drivers_nrf/nrf_soc_nosd # ??? Not sure

CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/softdevice/s140/headers/nrf52
# CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/softdevice/mbr/nrf52840/headers # ??? Not sure
CFLAGS += -I $(CURDIR)/../Common
CFLAGS += -I $(CURDIR)/../Common/nrf_SDK_files
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/crc32
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/softdevice/common/
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/bootloader/dfu/
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/experimental_section_vars

SOURCE_FILES = \
	main.c \
	Fota_crc_tool/fota_crc_tool.c \
	NRF_mbr/nrf_mbr_sd.c 

include $(LIBDIR)/Makefile.include
