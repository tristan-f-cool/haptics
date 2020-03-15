#!/bin/bash

#*** Precision Microdrives [PMD] ***
#* Haptic Feedback Evaluation Kit 2.0 [HFEK2]
#- Tristan Cool
#- July 2109

#***************************************************#
#    This script is run first. 						#
#	 -configures TFT/HDMI screen					#
#    -loads dependencies and clones firmware code.	#
#	 -creates a guest user with limited access.		#
#	 -executes firmware install script				#
#***************************************************#


echo "............. -PMD HFEK2- .................."
echo "load.bash"
cd ~

sudo apt-get update
sudo apt-get dist-upgrade

#Unzip and move toHFEK2 folder
sudo apt-get install unzip
cd Downloads
unzip toHFEK2.zip -d /home/pi/
cd ~
cd toHFEK2
mv PMD_logo.png PMD_logo2.jpg PMD_logo.png /home/pi/Pictures 

# TFT screen configuration
cd ~
echo "load.....TFT display - Adafruit 3.5 480x320"
wget https://raw.githubusercontent.com/adafruit/Raspberry-Pi-Installer-Scripts/master/adafruit-pitft.sh
chmod +x adafruit-pitft.sh
sudo ./adafruit-pitft.sh

sudo apt-get install mplayer


# SSH configuration
echo "load....SSH RealVNC"
cd ~
sudo apt-get install realvnc-vnc-server
sudo apt-get install realvnc-vnc-viewer

# Raspberry Pi - GPIO - Python
echo "load....Python3"
sudo apt-get install python3
#echo "load....GPIO"
sudo apt-get install python-dev python-rpi.gpio
#echo "load....PIP"
sudo apt-get install python3-pip
##echo "load....SPI"
sudo apt-get install python3-spidev python-spidev

# GITHUB repos code clone
echo "load.....GIT code"
sudo apt-get install git bc 
git clone https://github.com/pmd-hfek2/firmware.git
git config --global user.name "pmd-hfek2"

# Enable firmware
echo "load......firmware"
sudo apt-get install expect
cd firmware/firmware
sudo chmod +x install.bash
sudo chmod +x update.bash
sudo chmod +x update_auto.bash
sudo chmod +x test.bash
sudo chmod +x reset.bash
sudo chmod +x shutdown.bash
sudo chmod +x run.bash
sudo chmod +x start.bash

# Desktop shortcuts
ln -s /home/pi/firmware/firmware/reset.bash /home/pi/Desktop/
ln -s /home/pi/firmware/firmware/shutdown.bash /home/pi/Desktop/
#ln -s /home/pi/firmware/firmware/update.bash /home/pi/Desktop/
#ln -s /home/pi/firmware/firmware/test.bash /home/pi/Desktop/

# Create Guest user / remove Home access
cd ~
echo "load......guest user"
sudo adduser guest 
sudo chmod g+rw /home/guest
sudo chmod o-rwx /home/* 
sudo usermod -aG guest pi

echo "load.....COMPLETE"
echo "............................................"

## call install.bash
sudo sh firmware/firmware/install.bash
exit
