
SEARCH_DIR(.)
GROUP(-lgcc -lc_nano -lnosys)

MEMORY
{
/* Active region for code */
  MBR_SD (rx) :      ORIGIN = 0x00000000, LENGTH = 0x26000
  FLASH (rx) :       ORIGIN = 0x00026000, LENGTH = 0x58000
  
/* For temporary storage of new firmware */
  SWAP (rx) :        ORIGIN = 0x0007e000, LENGTH = 0x7f000

/* Two configuration areas (App data) */
  CONFIG2 (rx) :     ORIGIN = 0x000f5000, LENGTH = 0x01000
  CONFIG1 (rx) :     ORIGIN = 0x000f6000, LENGTH = 0x01000
  
/* Storage for device certificate (App data) */
  CERTIFICATE (rx) : ORIGIN = 0x000f7000, LENGTH = 0x01000

/* Bootloader */
  BLSETTINGS (rx) :  ORIGIN = 0x000ff000, LENGTH = 0x01000
  MBRSETTINGS (rx) : ORIGIN = 0x000fe000, LENGTH = 0x01000
  BOOTLOADER (rx) :  ORIGIN = 0x000f8000, LENGTH = 0x06000

/* RAM */
  SOFTDEVRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x022b0
  RAM (rwx) :        ORIGIN = 0x200022b0, LENGTH = 0x3dd50
}

__ApplicationStart = ORIGIN(FLASH);
__ApplicationEnd = ORIGIN(FLASH) + LENGTH(FLASH);

__BlSettingsStart = ORIGIN(BLSETTINGS);
__BlSettingsEnd = ORIGIN(BLSETTINGS) + LENGTH(BLSETTINGS);

/* Swap start/end not required, they are defined in nrf52_mira.ld */

INCLUDE "nrf52_mira.ld"
