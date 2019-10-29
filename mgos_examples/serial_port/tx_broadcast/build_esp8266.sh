#!/bin/bash

if [ "$1" == "clean" ]; then
    echo "*********************************"
    echo "*          BUILD CLEAN          *"
    echo "*********************************"
    rm -rf deps build
    cp mos.yml.esp8266 mos.yml
    mos build --clean --local --verbose --platform=esp8266
else
    mos build --local --verbose --platform=esp8266
fi
