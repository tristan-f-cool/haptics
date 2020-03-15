#!/bin/bash

#*** Precision Microdrives [PMD] ***
#* Haptic Feedback Evaluation Kit 2.0 [HFEK2]
#- Tristan Cool
#- July 2109

#***************************************************#
#    This script installs systems.				    #
#	 -executes test script							#
#***************************************************#

echo "............. -PMD HFEK2- .................."
echo "install.bash"

cd ~

sudo pip install --upgrade setuptools
sudo pip3 install --upgrade setuptools

# TFT touchscreen calibration
echo "install....TFT touchscreen"
echo "Copy: SUBSYSTEM==\"input\", ATTRS{name}==\"stmpe-ts\", ENV{DEVNAME}==\"*event*\", SYMLINK+=\"input/touchscreen\""
#SUBSYSTEM=="input", ATTRS{name}=="stmpe-ts", ENV{DEVNAME}=="*event*", SYMLINK+="input/touchscreen" 
echo "Exit: Ctrl + X ; Y"
sudo nano /etc/udev/rules.d/95-stmpe.rules
sudo rmmod stmpe_ts; sudo modprobe stmpe_ts
ls -l /dev/input/touchscreen
sudo apt-get install evtest tslib libts-bin
#sudo TSLIB_FBDEVICE=/dev/fb1 TSLIB_TSDEVICE=/dev/input/touchscreen ts_calibrate

# MPR121 Adafruit touchpad
echo "install...MPR121 touchpad"
sudo pip3 install adafruit-blinka
sudo pip3 install adafruit-circuitpython-mpr121
git clone https://github.com/adafruit/Adafruit_CircuitPython_MPR121.git
cd Adafruit_CircuitPython
sudo python3 setup.py install


# DRV2605 haptic driver
cd ~
sudo pip3 install adafruit-circuitpython-drv2605
git clone https://github.com/adafruit/Adafruit_CircuitPython_DRV2605.git
cd Adafruit_CircuitPython
sudo python3 setup.py

# Python pygame UI (Apps)
cd ~
sudo apt-get install python-pygame
sudo chmod +x installsdl.sh
sudo ./installsdl.sh
sudo pip3 install pygameui
git clone https://github.com/fictorial/pygameui.git
cd pygameui
sudo python3 setup.py

cd ~
# Desktop shortcup (Apps)
ln -s /home/pi/firmware/firmware/app/demo_app.py /home/pi/Desktop/

## call test.bash
cd firmware/firmware
sudo sh test.bash

echo "install.....COMPLETE"
echo "............................................"

exit
