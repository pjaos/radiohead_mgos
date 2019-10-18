## Serial Server Example

## Description
A RadioHead message is received (by this server) using a radio attached to a serial port using Mongoose OS. This should be used along with another device running the [serial_client](../serial_client/README.md) example.

## Building
Run the build scripts to build the code ('./build_esp32.sh clean' or './build_esp8266.sh clean') after installing the required software as detailed in the README.md file in the parent folder. Mongoose OS supports other processors and the code may work for these but only ESP32 and ESP8266 examples have been tested.

## Testing
This was tested with STX882/STRX882 radio modules connected to an ESP32 device and separate STX882/STRX882 radio modules connected to an ESP8266 device. Note that the radio is connected to the ESP32 device on Uart 1 and connected to the ESP8266 device on Uart 0. Uart 0 on the ESP32 device sends out debug data (shown below). Uart 1 on the ESP8266 device sends out debug data (also shown below).

The debug port on the device running the 'serial_client' example code is shown below.

```
main.cpp:21             send_msg_cb
main.cpp:25             send_msg_cb: Client TX
got reply from : 0x02: And hello back to you
main.cpp:21             send_msg_cb
main.cpp:25             send_msg_cb: Client TX
got reply from : 0x02: And hello back to you
```

The debug port on the device running the 'serial_server' example code is shown below.

```
got request from : 0x01: Hello World!
got request from : 0x01: Hello World!
got request from : 0x01: Hello World!
got request from : 0x01: Hello World!
```

## Radio Hardware
[STX882](../hardware/STX882_High-power_ASK_Transmitter_Module.pdf) and [SRX882](../hardware/SRX882_Micropower_superheterodyne_receiver_V2.1.pdf) (TX/RX modules) were used with this example code for testing.

## Serial Ports
 The same naming convention is used here to refer to Uarts and serial ports (Serial Port 0 = Uart 0, Serial port 1 = Uart 1, etc).

 The ESP32 has the following serial ports available.

```
 Uart 0
 RX  = GPIO3
 TX  = GPIO1
 CTS = GPIO19
 RTS = GPIO22
 
 Uart 1
 RX  = GPIO25
 TX  = GPIO26
 CTS = GPIO27
 RTS = GPIO13
 
 Uart 2
 RX  = GPIO16
 TX  = GPIO17
 CTS = GPIO14
 RTS = GPIO15
```

 The ESP8266 device has the following serial ports available.

```
 Uart 0
 RX  = GPIO3
 TX  = GPIO1
 
 Uart 1 (only the TXD line is available and is normally used for debug output)
 TX  = GPIO2
```