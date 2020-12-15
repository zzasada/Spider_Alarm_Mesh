# Launch Requirements V1
FOTA
Implement single pin A111 enable
Implement single pin A111 Alarm report
Allow A111 processor to accept OTA DFU directly
Implement UART between A111 processor and Mira Processor

Enable heartbeat
Enable Alarm reporting to gateway
Implement Alarm tone
Implement LED network status

# Version 2
    - Firmware 
        Implement Serial DFU on A111 processor
        Implement LIS3DH
        Implement BQ35100
        Possibly merge A111 firmware onto single processor. (XM112 freertos version)

    - Gateway
        Keep record of all connected devices
        Enable Cellular backup

    - Hardware
        Look at CORTEX M0 instead of nRF52840 if sticking with dual processors.

    