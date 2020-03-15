#*** Precision Microdrives [PMD] ***
#* Haptic Feedback Evaluation Kit 2.0 [HFEK2]
#- Tristan Cool
#- July 2019

#***************************************************#
#    This script is a system test.			        #
#***************************************************#

### --- TITLE ---
print("............. -PMD HFEK2- ..................")
print("test.py")

### --- IMPORT ---
import mpr121touch
#import drv2605

### --- SYSTEM ---


### --- FUNCTION ---


### --- DEBUG ---


### --- RUN ---
print("test.... MPR121 touchpad")
mpr121touch.mpr121_test()

#print("test.... DRV2605 haptic driver")
#drv2605.drv_test()

print("test.... COMPLETE")
print(".............................................")