# Example of FOTA transfer with Nordic's Secure Bootloader

This is an example over a complete Fota process including reflash of the application by a bootloader.
The bootloader in this project makes use of Nordic's Secure Bootloader with BLE as transport layer.

That means that this project can be modified to enable BLE updates in bootloader mode.

The source code for the Secure Bootloader can be found in the Nordic SDK. Also read more about it here to understand the process: https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v15.0.0%2Flib_bootloader.html


## The process
The mesh nodes will be polling their parent for a new firmware every ~5 minutes. If the parent responds that there is a new firmware, the download will start directly. Once the download is complete, it will be validated and if it is a different CRC compared to before, the update process will be triggered.

The update is performed by writting ```nrf_dfu_settings_t``` with information about the new firmware to the Bootloader Settings flash page‚ and then issue a reset. When the nRF52 chip starts up again it will enter the bootloader and see that there is a new firmware available, and flash it to the application area in flash. After this the new application will start up.


## Build
The commands are available in the ```Makefile```.

### Build the application

```
> make
```
Build and separate the application and the SoftDevice
```
> make app
```


### Build the bootloader
The bootlaoder is dependent of the Nordic 15.2 SDK. Download and change the path in the bootloader ```Makefile```.
```
> cd bootloader/pca10056_ble/armgcc/
> make
```

### Build Bootloader settings
Make sure to build and seperate the application first.
```
> make blsettings
```

## Flash the mesh node
In order for the mesh node to start up properly all parts needs to be flashed:
1. SoftDevice
2. Application
3. Bootloader
4. Bootloader settings
5. Certificate (MIND THE LOCATION IN THE LINKER SCRIPT)

The location of these files can be found in the linker script. One can also merge these hexfiles into one with ```hexmerger```, but it is not necessary.

## Using the example
Start by flashing a node and make sure it starts up. The example will give status messages over uart.
Start up the Border Gateway and make sure the mesh nodes joins the network. Compile a new firmware by defining ```NEW_VERSION``` and run ```make app```. Convert it to a bin file with
```
make bin
````
Copy to the firmwares folder of the border gateway. The Border Gateway should give you something like this example ```FOTA: Loaded slot 0, version 158```, when it has recognized and validated the file, and is now ready to be distributed.

Wait until you have a valid image at the mesh node. The transfer of this firmware takes roughly 1 minute at rate 0. Please mind that the mesh node polls the parent at an interval of ~5 minutes. When the image is received and is concluded to be a new version (different CRC) it will trigger a reflash.

Now the new application should start up with the added print statement: ```THIS IS A NEW VERSION!```.

## Security
Make sure to use your own keys! Create private and public keys with ```nrfutil```.
Default keys are included in this example, DO NOT USE FOR PRODUCTION.



# Disclaimer


Copyright (c) 2020, LumenRadio AB All rights reserved.

The software is provided as an example. The software is delivered without guarantees.

No guarantees is taken for protocol stability of the software, and future
updates may change API.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
