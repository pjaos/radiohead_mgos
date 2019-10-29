## Description

This board connects between the USB serial adapter and the ESP8266 debug board.
It powers the board from the USB +5v line. 
This allows the ESP8266 Uart 0 lines to be connected to the serial radio
devices (STX882 and STRX882).
Therefore this board must be removed to load code to the ESP8266 device 
and once loaded the board can be added with radios attached in order to
TX/RX over the radio link.