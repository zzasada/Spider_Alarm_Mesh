#include <mira.h>
#include <stdio.h>
#include <string.h>
#include "../Common/common.h"
#include <stdbool.h>
#include "Fota_crc_tool/fota_crc_tool.h"
#include "nrf_sdh_soc.h"
#include "nrf_dfu_types.h"

#define FOTA_FW_SLOT_ID 0

#define USING_RTT 1

extern uint8_t __ApplicationStart;
extern uint8_t __ApplicationEnd;
extern uint8_t __SwapStart;
extern uint8_t __BlSettingsStart;
uint32_t swap_start = 0x0007e000;
uint32_t bl_settings_start = 0x000ff000;

static struct fota_upgrade_struct{
    bool ready;
    uint32_t crc;
}fota_upgrade;

bool flash_is_busy = false;

static const mira_net_config_t net_config = {
    .pan_id = PAN_ID_COMMON,
    .key = KEY_COMMON,
    .mode = MIRA_NET_MODE_MESH,
    .rate = MIRA_NET_RATE_FAST,
    .antenna = 0,
    .prefix = NULL /* default prefix */
};

nrf_dfu_settings_t *flash_settings;

MIRA_IODEFS(
    MIRA_IODEF_NONE,    /* fd 0: stdin */
    // #ifdef USING_RTT
        MIRA_IODEF_RTT(0), /* fd 1: stdout */
    // #else
    //     MIRA_IODEF_UART(0),
    // #endif
    MIRA_IODEF_NONE     /* fd 2: stderr */
    /* More file descriptors can be added, for use with dprintf(); */
);

PROCESS(main_proc, "Main process");
PROCESS(fota_upgrade_proc, "FOTA Process");

static void fota_sd_evt_observer(uint32_t evt_id, void *context)
{
    if (!flash_is_busy) {
        return;
    }

    if (evt_id == NRF_EVT_FLASH_OPERATION_SUCCESS) {
        flash_is_busy = false;
    } else if (evt_id == NRF_EVT_FLASH_OPERATION_ERROR) {
        printf("Flash operation failed!\n");
        flash_is_busy = false;
    } else {
        // Non-flash related event, ignore
    }

    process_poll(&fota_upgrade_proc);
}

static uint32_t calc_settings_crc(nrf_dfu_settings_t *settings){
    uint32_t crc_state;
    mira_size_t image_size = settings->bank_1.image_size;
    uint8_t version_no = settings->app_version;
    mira_size_t i;
    mira_size_t j;
    uint8_t buffer[32];

    fota_crc_init(&crc_state);

    for (i = 0; i < image_size; i += 32) {
        mira_size_t block_size = 32;
        if (image_size - i < block_size) {
            block_size = image_size - i;
        }
        for (j = 0; j < block_size; j++) {
            buffer[j] = (i + j + version_no) % 77;
        }
        fota_crc_update(&crc_state, buffer, block_size);
    }
    return crc_state;
}

int fota_app_init(uint32_t version)
{
    printf("FOTA_app_init()\n");
    flash_settings = (nrf_dfu_settings_t *) (&__BlSettingsStart);

    uint32_t application_max_size;

    // Ensure that the data in the settings page is correct
    application_max_size = (uint32_t) &__ApplicationEnd - (uint32_t) &__ApplicationStart;
    if (flash_settings->bank_0.image_size > application_max_size) {
        printf("Bootloader settings on flash is corrupt image_size=%lu, app_max_size=%lu\n", flash_settings->bank_0.image_size, application_max_size);
        // return -1;
    }else{
        printf("Application max size is ok\n");
    }

    // Ensure that what's in the bootloader settings page matches the calculated CRC
    if (flash_settings->crc != calc_settings_crc(flash_settings)) {
        printf("Bootloader settings CRC does not match\n");
        // return -1;
    }else{
        printf("Bootloader settings CRC match\n");
    }

    if (mira_fota_init() != MIRA_SUCCESS) {
        printf("Failed to initialize Mira FOTA\n");
        return -1;
    }else{
        printf("Mira FOTA initialized.\n");
    }

    return 0;
}


void mira_setup(void)
{
    mira_status_t rtt_ret;
    rtt_ret = mira_rtt_init();
    if(rtt_ret != MIRA_SUCCESS){ /*TODO: figure out how to report this */ }

    MIRA_MEM_SET_BUFFER(12288);

    printf("Version 1\n");
    process_start(&main_proc, NULL);
    // process_start(&fota_upgrade_proc, NULL);
}

const char *net_state(void)
{
    switch (mira_net_get_state()) {
    case MIRA_NET_STATE_NOT_ASSOCIATED:
        return "not associated";

    case MIRA_NET_STATE_IS_COORDINATOR:
        return "is coordinator";

    case MIRA_NET_STATE_ASSOCIATED:
        return "associated";

    case MIRA_NET_STATE_JOINED:
        return "joined";
    }
    return "unknown";
}

PROCESS_THREAD(main_proc, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();

    mira_status_t result = mira_net_init(&net_config);
    if (result) {
        printf("FAILURE: mira_net_init returned %d\n", result);
        while (1);
    }

    int success = fota_app_init(0);
    if(success == 0){
        printf("FOTA initialized successfully.\n");
    }else{
        printf("FOTA failed to initialize.\n");
    }

    NRF_SDH_SOC_OBSERVER(m_fota_sd_evt_observer, 0, fota_sd_evt_observer, NULL);

    while (1) {
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if (mira_fota_is_valid(FOTA_FW_SLOT_ID)) {
            printf("%s, Valid image: %ld bytes, version %d\n", net_state(), mira_fota_get_image_size(FOTA_FW_SLOT_ID), mira_fota_get_version(FOTA_FW_SLOT_ID));
            fota_upgrade.ready = true;
            // process_poll(&fota_upgrade_proc);
            process_start(&fota_upgrade_proc, NULL);
        } else {
            printf("%s, No valid image available in cache\n", net_state());
        }
    }

    PROCESS_END();
}

PROCESS_THREAD(fota_upgrade_proc, ev, data)
{
    uint32_t num_words;

    static nrf_dfu_settings_t new_settings;

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    while (1) {
        PROCESS_WAIT_UNTIL(fota_upgrade.ready == true);
        fota_upgrade.ready = false;

        if (flash_settings->bank_0.image_crc == fota_upgrade.crc) {
            printf("Firmware already installed\n");
            continue;
        }else{
            printf("Firmware not already installed\n");
        }
        // Lock the FOTA memory by starting a read session
        if (mira_fota_read_start(FOTA_FW_SLOT_ID) != MIRA_SUCCESS) {
            printf("Failed to lock FOTA buffer\n");
            continue;
        }else{
            printf("FOTA buffer locked\n");
        }
        PROCESS_WAIT_WHILE(mira_fota_is_working());

        new_settings = *flash_settings;
        new_settings.bank_1.image_crc = fota_upgrade.crc;
        new_settings.bank_1.image_size = mira_fota_get_image_size(FOTA_FW_SLOT_ID);
        new_settings.bank_1.bank_code = NRF_DFU_BANK_VALID_APP;
        new_settings.progress.update_start_address = (uint32_t) &__SwapStart + MIRA_FOTA_HEADER_SIZE;
        new_settings.crc = calc_settings_crc(&new_settings);

        flash_is_busy = true;
        sd_flash_page_erase(((uint32_t) &__BlSettingsStart) >> 12);
        PROCESS_YIELD_UNTIL(!flash_is_busy);

        printf("Settings page erased\n");

        flash_is_busy = true;
        num_words = (sizeof(new_settings) + 3) / sizeof(uint32_t);
        sd_flash_write((uint32_t *) &__BlSettingsStart, (uint32_t *) &new_settings, num_words);
        PROCESS_YIELD_UNTIL(!flash_is_busy);

        printf("New settings written, restarting application\n");

        mira_sys_reset();
    }

    PROCESS_END();
}