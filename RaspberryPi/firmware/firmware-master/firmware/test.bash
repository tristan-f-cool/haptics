#!/bin/bash

#*** Precision Microdrives [PMD] ***
#* Haptic Feedback Evaluation Kit 2.0 [HFEK2]
#- Tristan Cool
#- July 2109

#***************************************************#
#    This script is a system test.			        #
#***************************************************#

echo "............. -PMD HFEK2- .................."
echo "test.bash"

#TFT screen test
echo "test....TFT touchscreen"
lxterminal -e evtest /dev/input/touchscreen

#System test
sudo python firmware/firmware/test.py

echo "test.....COMPLETE"
echo "............................................"

exit
