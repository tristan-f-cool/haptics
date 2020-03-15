#!/bin/bash

#*** Precision Microdrives [PMD] ***
#* Haptic Feedback Evaluation Kit 2.0 [HFEK2]
#- Tristan Cool
#- July 2109

#************************************************************#
#   This script is a password protected firmware update.     #
#************************************************************#

echo "............. -PMD HFEK2- .................."
echo "update.bash"

DATE=$(date +"%Y-%m-%d_%H%M")

cd firmware/firmware
git status
git pull origin master

echo "HFEK2 updated: "
echo $DATE
echo "............................................"

exit
