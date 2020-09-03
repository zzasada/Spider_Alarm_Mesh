# Spider_Alarm_Mesh
Code holding production mesh and Test Modules for Spider Mesh pcb

# Create and flash license
nrfjprog --eraseall
nrfjprog --ids (50130153)
python mira_license.py get_device_id -P nrfjprog -s 50130153 (4787ca1fbb2be7c4)
python mira_license.py license -d COM84 -i 4787ca1fbb2be7c4 -P nrfjprog -s 50130153 -a 0xff000 -l 0x1000
