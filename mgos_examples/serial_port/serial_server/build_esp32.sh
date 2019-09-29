#!/bin/bash

if [ "$1" == "clean" ]; then
    echo "*********************************"
    echo "*          BUILD CLEAN          *"
    echo "*********************************"
    rm -rf deps build
    cp mos.yml.esp32 mos.yml
    mos build --clean --local --verbose --platform=esp32
else
    mos build --local --verbose --platform=esp32
fi
