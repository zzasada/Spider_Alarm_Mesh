# Alarm
# Info
    RED - Inventory mode
    BLUE - Searching for network
    MAGENTA - Found network, attempting to connect
    GREEN - Connected to network
    
    WHITE -
    YELLOW -
    ORANGE -
    CYAN -

    set device name to "test" with nfc to run self test
    set device name to "-" to put in inventory mode
    set devine name to anything else will start device with default credentials.

# UDP Ports
    1000 General all listen

# Change Log
    # V1
        -Implement LIS3DH
        -Implement BQ35100
        -Get temperature from BQ35100
        -Allow NFC writing of PAN_ID and NETWORK_KEY
        -Enable BLE connection

        -Evaluate NFC antenna and A111 on one side of pcb and all components on otherside

    # V0 (479 uA inventory mode current)
        - COMPLETE:
            - NFC mode configure (done)
            - Connect to gateway (done)
            - Heartbeat (done)
            - Measure sleep current in NFC standby (done) 479 uA (4.5 years in inventory mode.)

        -TODO:
            - Fix NFC when building for nRF52840-os
            - Communicate with A111
            - Implement Motion detection
            - Implement FOTA
            - Write self test
            - ARM / Disarm Functionality
            - Evaluate Panasonic PIR