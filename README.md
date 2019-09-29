# Using the RadioHead library with Mongoose OS

## Overview
The RadioHead packet radio library for embedded processors supports many low cost radio devices.
The Mongoose OS IoT firmware development framework provides seamless integration of several 
devices (ESP32/ESP8266, TICC 3220/3200, STM32 L4/F4/F7). This repo provides working examples
of the RadioHead library used in conjunction with Mongoose OS.

## Usage
 The top level of the repo contains the RadioHead library as a git submodule. Therefore use the --recurse-submodules git command line option when cloning the radiohead_mgos.git repo.
 
 Before compiling the examples you'll need to install the mos tool as detailed at https://mongoose-os.com/docs/mongoose-os/quickstart/setup.md.
 
 Under the mgos__examples folder you will example folders (E.G serial_port/serial_tx_broadcast). These contain working examples each of which contain a README.md file providing more information. These example code folders are derived from the RadioHead/examples folder. Therefore if no example exists in mgos_examples check the RadioHead/examples folder and you may find example code that will help you generate a new example under mgos__examples. If you create new examples please raise a github pull request to add your example so that others may benefit from your efforts.

## Acknowledgements
  Many thanks to airspayce and all contributors to the RadioHead library (http://www.airspayce.com/mikem/arduino/RadioHead/).

  Many thanks to Cesanta and all the contibutors to the Mongoose OS IoT firmware development framework (https://mongoose-os.com).

Please note that both of the above have Open Source and Commercial Licenses.
