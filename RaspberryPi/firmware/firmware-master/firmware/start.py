#*** Precision Microdrives [PMD] ***
#* Haptic Feedback Evaluation Kit 2.0 [HFEK2]
#- Tristan Cool
#- August 2019

#***************************************************#
#    This script runs at boot. System start.        #
#***************************************************#

### --- TITLE ---
print("............. -PMD HFEK2- ..................")
print("start.py")

### --- IMPORT ---
import drv2605 as drv


### --- SYSTEM ---
#DRV2605 activate pulse
drv.drv_set_motor(1) #LRA
drv.drv_effect_run(5) #click

### --- FUNCTION ---


### --- DEBUG ---


### --- RUN ---

print("...........................................")
