# Testing Performed

In order to verify that the changes to the Radiohead library to support Mongoose OS
function correctly testing was performed. This testing involved the following modules.
These were chosen as the exercise the use of a radio connected to a serial port, a 
radio connected to the SPI bus without the use of interrupts and a radio connected 
to the SPI bus with the use of interrupts.

The testing involved checking that two test scenarios function correctly..

1. Broadcast messages
This checks that uni directional communication functions correctly.
The messages shown below (serial port output) are from the tx_broadcast 
and rx_broadcast Mongoose OS projects respectively.

```
main.cpp:28             tx_broadcast_poll_timer_cb: Client TX
main.cpp:28             tx_broadcast_poll_timer_cb: Client TX
main.cpp:28             tx_broadcast_poll_timer_cb: Client TX
main.cpp:28             tx_broadcast_poll_timer_cb: Client TX
```

```
main.cpp:25             rx_broadcast_poll_timer_cb: Client RX
main.cpp:26             from:  01
main.cpp:27             to:    ff
main.cpp:28             id:    0d
main.cpp:29             flags: 00
main.cpp:30             rx_buf:Hello World!
```

2. Client server messages
This checks that bi-directional communication functions correctly.
The messages shown below (serial port output) are from the client 
and server Mongoose OS projects respectively.

```
main.cpp:21             send_msg_cb
main.cpp:25             send_msg_cb: Client TX
got reply from : 0x02: And hello back to you
main.cpp:21             send_msg_cb
main.cpp:25             send_msg_cb: Client TX
got reply from : 0x02: And hello back to you
```

```
got request from : 0x01: Hello World!
got request from : 0x01: Hello World!
got request from : 0x01: Hello World!
got request from : 0x01: Hello World!
```
## Results

For each of the test scenarios below the broadcast and client server
testing was performed. The definition of a test pass was that the 
above message sequences occurred.

1 serial_port radio device
 + Broadcast: TX=ESP8266, RX=ESP32 = PASS
 + Broadcast: TX=ESP32, RX=ESP8266 = PASS
 + Client Server: Client=ESP8266, Server=ESP32 = PASS
 + Client Server: Client=ESP32, Server=ESP8266 = PASS

2 nRF905 radio device
 + Broadcast: TX=ESP8266, RX=ESP32 = PASS
 + Broadcast: TX=ESP32, RX=ESP8266 = PASS
 + Client Server: Client=ESP8266, Server=ESP32 = PASS
 + Client Server: Client=ESP32, Server=ESP8266 = PASS

3 CC1101 radio device
 + Broadcast: TX=ESP8266, RX=ESP32 = PASS
 + Broadcast: TX=ESP32, RX=ESP8266 = PASS
 + Client Server: Client=ESP8266, Server=ESP32 = PASS
 + Client Server: Client=ESP32, Server=ESP8266 = PASS
