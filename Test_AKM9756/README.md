# Alarm
 Firmware running on mesh nodes for S55A, S11MS, and S33DL

# Info
 .pan_id = 0x8724a685,
 .key = {
        0x93, 0x30, 0x0c, 0x4b,
        0x3a, 0xd1, 0x0f, 0x8b,
        0x69, 0xc3, 0xa1, 0x17,
        0x98, 0x5f, 0xc0, 0x47
 },

    Pin 1.10 Alert ()
    Pin 1.06 LIS3DL Int 1
    Pin 0.29 LIS3DL CS   CS
    Pin 0.08 LIS3DL MISO SA0/SDO
    Pin 0.24 Batt_Alarm
    Pin 1.02 LIS3DL MOSI SDA/SDI/SDO (incorrect on Schematic)
    Pin 1.01 LIS3DL SCK  SCL/SPC (incorrect on Schematic)
    Pin 1.05 Alarm_Enable
    Pin 1.07 Red
    Pin 0.20 Blue
    Pin D- **************
    Pin 0.12 Green
    Pin 0.26 SDA
    Pin 0.27 SCL

# UDP Ports
    1000 General all listen

# TODO:
    Enable Alarm
    Disable Alarm
    Test Alarm

