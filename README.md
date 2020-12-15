# Spider_Alarm_Mesh
Code holding production mesh and Test Modules for Spider Mesh pcb

# Helpful git command to change the file permission stored in git
git update-index --chmod=+x path/to/file

# Create and flash license
nrfjprog --memrd 0x10000100 (reads out module type)
nrfjprog --memrd 0x10000104 (reads out manufacturing revision)
nrfjprog --eraseall
nrfjprog --ids (50130153)
python mira_license.py get_device_id -P nrfjprog -s 50130153 (4787ca1fbb2be7c4)
python mira_license.py license -d COM84 -i 4787ca1fbb2be7c4 -P nrfjprog -s 50130153 -a 0xff000 -l 0x1000
python mira_license.py license -d COM84 -i ce7bdbe198cfb538 -P nrfjprog -s 50130153 -a 0xff000 -l 0x1000
python mira_license.py license -d COM84 -i b3063c1949e35d73 -P nrfjprog -s 683934541 -a 0xff000 -l 0x1000
python mira_license.py license -d COM84 -i 7c687c5bddd911c2 -P nrfjprog -s 683934541 -a 0xff000 -l 0x1000
python mira_license.py license -d COM84 -i f43d2070b90264bb -P nrfjprog -s 50130153 -a 0xff000 -l 0x1000

python mira_license.py license -I 6c45fbc3d69d32d2.lic -a 0xf7000 -l 0x1000 -i 6c45fbc3d69d32d2

python mira_license.py generate -d COM84 -i 06687d27393195b9 -F licenses/06687d27393195b9.lic
python mira_license.py license -I licenses/06687d27393195b9.lic -a 0xf7000 -l 0x1000 -i 06687d27393195b9

python mira_license.py license -I b3063c1949e35d73.lic -a 0xff000 -l 0x1000 -i b3063c1949e35d73

# Memory Allocation

0x0000 - 0x0A17 - bootloader/mbr
0x1000 - 0x140F3 - Loaded Application
0xff000 - 0xff0f4 - 245 bytes for license
0xffff8 - 0xfffff - Not sure ....

0x7F000 - 0x93117 is Fota 82200 in example location

Total
0x0000 - 0x00100000 = 1048576 bytes total

possibly modify ORIGIN in nrf52840_xxaa.ld in platform/nrf52840-os/nrf52840_xxaa.ld

# Flash with License file
nrfjprog --family nrf52 --eraseall
./mira_license.py get_device_id -P nrfjprog -s 50130153
./mira_license.py generate -d COM84 -i 421322faf0b36442 -F 421322faf0b36442.lic
(0xff000 for nrf52840-os) ./mira_license.py license -I 421322faf0b36442.lic -a 0xf7000 -l 0x1000 -i 421322faf0b36442
nrfjprog --family nrf52 --sectorerase --program ***.hex --reset --verify