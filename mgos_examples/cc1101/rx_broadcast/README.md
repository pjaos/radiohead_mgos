## CC1101 RX Broadcast Example

## Description
A RadioHead broadcast message is received using a radio attached to the SPI interface using Mongoose OS. This should be used along with another device running the [tx_broadcast](../tx_broadcast/README.md) example.

## Building
Run the build scripts to build the code ('./build_esp32.sh clean' or './build_esp8266.sh clean') after installing the required software as detailed in the README.md file in the parent folder. Mongoose OS supports other processors and the code may work for these but only ESP32 and ESP8266 examples have been tested.

## Testing
This was tested with a CC1101 radio module connected to an ESP32 device and a separate CC1101 radio module connected to an ESP8266 device.

The debug port on the device running the 'tx_broadcast' example code is shown below.

```
main.cpp:28             tx_broadcast_poll_timer_cb: Client TX
main.cpp:28             tx_broadcast_poll_timer_cb: Client TX
main.cpp:28             tx_broadcast_poll_timer_cb: Client TX
main.cpp:28             tx_broadcast_poll_timer_cb: Client TX
```

The debug port on the device running the 'rx_broadcast' example code is shown below.

```
main.cpp:25             rx_broadcast_poll_timer_cb: Client RX
main.cpp:26             from:  01
main.cpp:27             to:    ff
main.cpp:28             id:    0d
main.cpp:29             flags: 00
main.cpp:30             rx_buf:Hello World!
```

## Radio Hardware
[nRF905 device datasheet](../hardware/cc1101_datasheet.pdf). The module with this device fitted had the pinout [CC1101](../hardware/cc1101_pinout.png).

## SPI Interface
 The CC1101 modules SPI interface was connected to the ESP32 device as shown below.

```
 ESP32      CC1101
 ---------|----------
 CPIO18   - SCLK
 GPIO19   - SO
 GPIO23   - SI
 GPIO5    - CSn
 GPIO4    - GDO0
```

 The CC1101 modules SPI interface was connected to the ESP8266 device as shown below.

```
 ESP8266    CC1101
 ---------|----------
 CPIO14   - SCLK
 GPIO12   - SO
 GPIO13   - SI
 GPIO5    - CSn
 GPIO4    - GDO0
```