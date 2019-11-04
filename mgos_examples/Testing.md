# Testing Performed

In order to verify that the changes to the Radiohead library to support Mongoose OS
function correctly testing was performed. This testing involved the following modules.
These were chosen as the exercise the use of a radio connected to a serial port, a 
radio connected to the SPI bus without the use of interrupts and a radio connected 
to the SPI bus with the use of interrupts.

## Results

The following testing was performed. A test pass was recorded if the 
above message sequences were displayed on the serial port of the 
respective device indicating that messages were passed correctly.

At Mon Nov 4 05:24:52 2019 the following testing was performed.

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
 
4 - Compile RadioHead/examples using Arduino IDE as defined in the extended test method below.
PASS
 

At Wed Oct 30 06:04:09 2019 the following testing was performed.

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


At Tue Oct 29 19:14:21 2019 the following testing was performed.

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

# Testing Method

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
## Extended Test Method (3 Nov 2019)

After git hash 3ea3ad13ae58f1c283d732745c62d218639bb6b9 an extra test step was added to check the the modified RadioHead library still compiled under the Arduino IDE.

- Install the Arduino IDE if not already installed.
    - Install xz-utils.
    E.G
      'sudo apt-get install xz-utils'
    - From https://www.arduino.cc/en/Main/Software download the 64 bit Linux version of the IDE.
    - Decompress the file.
    E.G
    'tar -xvf  arduino-1.8.10-linux64.tar.xz'
- Remove any old version of the Arduino RadioHead library
E.G
'rm -rf ~/Arduino/libraries/RadioHead'
- Copy the new RadioHead folder into ~/Arduino/libraries/RadioHead
- Start the Arduino IDE
- Test that the RadioHead library examples can be compiled in the Arduino IDE.

  To do this open each of the following, press Ctrl R
  
```
examples/nrf905/nrf905_client/nrf905_client.pde
examples/nrf905/nrf905_server/nrf905_server.pde
examples/nrf905/nrf905_reliable_datagram_client/nrf905_reliable_datagram_client.pde
examples/nrf905/nrf905_reliable_datagram_server/nrf905_reliable_datagram_server.pde
examples/cc110/cc110_client/cc110_client.pde
examples/cc110/cc110_server/cc110_server.pde
```

- Do the same for the following except that when the source file is loaded change the following line (no need to save the file) before pressing Ctrl R. 

```
 RH_Serial driver(Serial1); ->  RH_Serial driver(Serial);
```

```
examples/serial/serial_reliable_datagram_client/serial_reliable_datagram_client.pde
examples/serial/serial_reliable_datagram_server/serial_reliable_datagram_server.pde
```
