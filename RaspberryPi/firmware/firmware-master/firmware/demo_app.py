#*** Precision Microdrives [PMD] ***
#* Haptic Feedback Evaluation Kit 2.0 [HFEK2]
#- Tristan Cool
#- July 2019

#***************************************************#
#    This script runs the touchpad demo app         #
#***************************************************#

### --- TITLE ---
print("............. -PMD HFEK2- TOUCHPAD DEMO APP_0 (TOUCH) ..................")
print("demo_app.py")

### --- IMPORT ---
import mpr121touch as mpr
import drv2605 as drv

### --- SYSTEM ---
## - DRV2605 haptic
drv.set_motor(1) #LRA

### --- FUNCTION ---
def run():
    #touchpad input
    touched = mpr.mpr121_run()

    #single click/strong, double click/strong
    effects = (5,25,35,36)
    current_effect

    #PLAY or SELECT buttons
    if touched[2] or touched[3]:
        #play current effect
        if current_effect not in (5,25,35,36):
            current_effect = effects[0]
        drv.drv_effect_run(current_effect)

    #PMD button
    if touched[4]:
        #vary effect + play effect
        if current_effect == effects[0]:
            current_effect == effects[1]
        elif current_effect == effects[1]:
            current_effect == effects[2]
        elif current_effect == effects[2]:
            current_effect == effects[3]
        elif current_effect == effects[3]:
            current_effect == effects[0]
        drv.drv_effect_run(current_effect)

### --- DEBUG ---


### --- RUN ---
while True:
    run()

print("run....stopping")
print("...........................................")
