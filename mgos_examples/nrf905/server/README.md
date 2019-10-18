## NRF905 server Example

## Description
A RadioHead message is received using a radio attached to the SPI interface using Mongoose OS. This should be used along with another device running the [server](../client/README.md) example.

## Building
Run the build scripts to build the code ('./build_esp32.sh clean' or './build_esp8266.sh clean') after installing the required software as detailed in the README.md file in the parent folder. Mongoose OS supports other processors and the code may work for these but only ESP32 and ESP8266 examples have been tested.

## Testing
This was tested with an nRF905 radio module connected to an ESP32 device and a separate nRF905 radio module connected to an ESP8266 device.

The debug port on the device running the 'client' example code is shown below.

```
main.cpp:21             send_msg_cb
main.cpp:25             send_msg_cb: Client TX
got reply from : 0x02: And hello back to you
main.cpp:21             send_msg_cb
main.cpp:25             send_msg_cb: Client TX
got reply from : 0x02: And hello back to you
```

The debug port on the device running the 'server' example code is shown below.

```
got request from : 0x01: Hello World!
got request from : 0x01: Hello World!
got request from : 0x01: Hello World!
got request from : 0x01: Hello World!
```

## Radio Hardware
[nRF905 device datasheet](../hardware/nRF905_PS_v1.5.pdf). The module with this device fitted had the pinout [nRF905](../hardware/module_pinout.jpeg).

## SPI Interface
 The nRF905 modules SPI interface was connected to the ESP32 device as shown below.

```
 ESP32      NRF905
 ---------|----------
 GPIO16   - TRX_CE
 CPIO18   - SPI_CLK
 GPIO19   - SPI_MISO
 GPIO23   - SPI_MOSI
 GPIO5    - SPI_CS
 GPIO4    - TX_EN
            PWR_UP pulled to VCC through 10K resistor.
```

 The nRF905 modules SPI interface was connected to the ESP8266 device as shown below.

```
 ESP8266    NRF905
 ---------|----------
 GPIO16   - TRX_CE
 CPIO14   - SPI_CLK
 GPIO12   - SPI_MISO
 GPIO13   - SPI_MOSI
 GPIO5    - SPI_CS
 GPIO4    - TX_EN
            PWR_UP pulled to VCC through 10K resistor.
```