## NRF905 RX Broadcast Example

## Description
A RadioHead broadcast message is received using a radio attached to the SPI interface using Mongoose OS. This should be used along with another device running the [tx_broadcast](../tx_broadcast/README.md) example.

## Building
Run the build scripts to build the code ('./build_esp32.sh clean' or './build_esp8266.sh clean') after installing the required software as detailed in the README.md file in the parent folder. Mongoose OS supports other processors and the code may work for these but only ESP32 and ESP8266 examples have been tested.

## Testing
This was tested with an nRF905 radio module connected to an ESP32 device and a separate nRF905 radio module connected to an ESP8266 device.

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