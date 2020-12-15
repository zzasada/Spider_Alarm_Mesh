PROJECT_NAME = Test_A111

TARGET ?= nrf52840-os
LIBDIR ?= $(CURDIR)/../../..

CFLAGS += -DNRF52840_XXAA
CFLAGS += -I $(CURDIR)
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/modules/nrfx/mdk
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/toolchain/cmsis/include
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/delay
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/util
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/softdevice/s140/headers
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/modules/nrfx
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/modules/nrfx/hal
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/modules/nrfx/drivers/include
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/integration/nrfx
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/integration/nrfx/legacy
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/log
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/experimental_section_vars
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/strerror
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/log/src
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/pwr_mgmt
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/mutex
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/atomic
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/memobj
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/balloc
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/components/libraries/ringbuf
CFLAGS += -I $(CURDIR)/../../../nRF5SDK/external/fprintf
CFLAGS += -I $(CURDIR)/../Common
CFLAGS += -I $(CURDIR)/../Common/nrf_SDK_files
CFLAGS += -I $(CURDIR)/../Common/acconeer
CFLAGS += -I $(CURDIR)/../Common/acconeer/include
CFLAGS += -I $(CURDIR)/../Common/acconeer/lib
CFLAGS += -I $(CURDIR)/../Common/acconeer/source

# Libraries common to all targets and the target specific library libacc_rf_certification_test.a
LIB_FILES += \
    -Wl,--start-group \
  $(CURDIR)/../Common/acconeer/lib/libacc_detector_distance.a \
  $(CURDIR)/../Common/acconeer/lib/libacc_detector_presence.a \
  $(CURDIR)/../Common/acconeer/lib/libacc_rf_certification_test.a \
  $(CURDIR)/../Common/acconeer/lib/libacconeer.a \
    -Wl,--end-group 

SOURCE_FILES = \
  ../Common/acconeer/source/acc_app_integration_xm122.c \
  ../Common/acconeer/source/acc_app_integration_xm122.c \
  ../../../nRF5SDK/modules/nrfx/drivers/src/nrfx_rtc.c \
  ../../../nRF5SDK/components/libraries/util/app_error.c \
  ../../../nRF5SDK/components/libraries/util/app_util_platform.c \
  ../../../nRF5SDK/components/libraries/util/app_error.c \
  ../../../nRF5SDK/components/libraries/util/app_error_weak.c \
    ../../../nRF5SDK/components/libraries/log/src/nrf_log_frontend.c \
  ../../../nRF5SDK/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c \
	a111/a111.c \
  Main.c

include $(LIBDIR)/Makefile.include
