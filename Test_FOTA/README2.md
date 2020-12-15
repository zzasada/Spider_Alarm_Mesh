# Flashing instructions
- nrfjprog --eraseall
- Currently using nRF Connect Programmer (bootloader, blsettings, app, softdevice) then load certificate seperately

- nrfjprog --ids (683715826)
- python mira_license.py get_device_id -P nrfjprog -s 683715826 (6c45fbc3d69d32d2)
- python mira_license.py generate -d COM84 -i 6c45fbc3d69d32d2 -F licenses/6c45fbc3d69d32d2.lic
- python mira_license.py license -I licenses/6c45fbc3d69d32d2.lic -a 0xf7000 -l 0x1000 -i 6c45fbc3d69d32d2