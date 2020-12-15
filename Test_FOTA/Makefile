PROJECT_NAME = fota_receiver

TARGET ?= nrf52840ble-os
TARGET_APP ?= nrf52840ble-os-app
LIBDIR ?= f:/Mira/libmira-2.4.2/libmira
SDK_DIR ?= $(LIBDIR)/nRF5SDK

SOURCE_FILES = \
	fota_receiver.c \
	fota_update.c \
	crc/crc.c


CFLAGS += -I$(CURDIR)/crc

CFLAGS += -I$(SDK_DIR)/components/softdevice/s140/include
CFLAGS += -I$(SDK_DIR)/components/softdevice/s140/headers
CFLAGS += -I$(SDK_DIR)/components/softdevice/mbr/nrf52840/headers
CFLAGS += -I$(SDK_DIR)/config/nrf52840/config/
CFLAGS += -DNRF52840_XXAA
LDSCRIPT = $(CURDIR)/nrf52840_fota.ld

CFLAGS += -I$(SDK_DIR)/components/libraries/bootloader/dfu
CFLAGS += -I$(SDK_DIR)/components/libraries/util

CFLAGS += -I$(SDK_DIR)/components/softdevice/common/
CFLAGS += -I$(SDK_DIR)/components/toolchain/cmsis/include
CFLAGS += -I$(SDK_DIR)/modules/nrfx/mdk
CFLAGS += -I$(SDK_DIR)/components/libraries/experimental_section_vars

# CFLAGS += -I$(SDK_DIR)/external/mbedtls/include
# CFLAGS += -I$(SDK_DIR)/integration/nrfx
# CFLAGS += -I$(SDK_DIR)/modules/nrfx
# CFLAGS += -I$(SDK_DIR)/modules/nrfx/drivers/include
# CFLAGS += -I$(SDK_DIR)/modules/nrfx/hal

include $(LIBDIR)/Makefile.include

app: $(PROJECT_NAME)-$(TARGET).hex
	rm -f $(PROJECT_NAME)-$(TARGET_APP).hex
	hexmerge.py $<:0x00026000:0x7DFFF -o $(PROJECT_NAME)-$(TARGET_APP).hex

blsettings:
	rm -f $(PROJECT_NAME)-$(TARGET_APP)-blsettings.hex
	nrfutil settings generate --family NRF52840 --application $(PROJECT_NAME)-$(TARGET_APP).hex --application-version 0x1 --bootloader-version 0x1 --bl-settings-version 1 --key-file private.key $(PROJECT_NAME)-$(TARGET_APP)-blsettings.hex

clean_all:
	$(MAKE) clean
	rm -f 0.bin
	rm -f fota_receiver $(PROJECT_NAME)-$(TARGET_APP)-blsettings.hex
	rm -f fota_receiver $(PROJECT_NAME)-$(TARGET_APP).hex

bin:
	rm -f 0.bin
	arm-none-eabi-objcopy -I ihex -O binary $(PROJECT_NAME)-$(TARGET_APP).hex 0.bin