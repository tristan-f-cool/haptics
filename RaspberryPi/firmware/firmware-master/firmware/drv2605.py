#*** Precision Microdrives [PMD] ***
#* Haptic Feedback Evaluation Kit 2.0 [HFEK2]
#- Tristan Cool
#- July 2019

#***************************************************#
#    This script is the DRV2605	haptic driver		#
#***************************************************#

### --- TITLE ---
print("............. -PMD HFEK2- ..................")
print("drv2605.py")

### --- IMPORT ---
import time
import board
import busio
import subprocess
import adafruit_drv2605

### --- SYSTEM ---
#I2C bus
i2c = busio.I2C(board.SCL, board.SDA)

#DRV2605
drv = adafruit_drv2605.DRV2605(i2c)

### --- FUNCTION ---
def drv_run():
    effect_id = 1
    print('DRV2605 haptic effect: #{0}'.format(effect_id))
    # Set the effect on slot 0.
    drv.sequence[0] = adafruit_drv2605.Effect(effect_id)
    # (assign effects to up to 7 different slots to combine)
    # drv.sequence[1] = adafruit_drv2605.Pause(0.5)  # Pause for half a second
    drv.play()
    time.sleep(0.5)
    drv.stop()
    # Increment effect ID and wrap back to 1.
    effect_id += 1
    if effect_id > 123:
        effect_id = 1

def drv_lra_run():
    drv.use_LRM()
    drv_run()

def drv_erm_run():
    drv.use_ERM()
    drv_run()

def drv_set_motor(motor_type=1):
    #motor_type: 0=ERM ; 1=LRA
    if(motor_type == 1):
        drv.use_LRM()
    else:
        drv.use_ERM()

def drv_set_effect(effect_id=1):
    #effect_id: 1~123
    if(effect_id < 1):
        effect=1
    elif(effect_id > 123):
        effect_id=123
    return effect_id

def drv_effect_run(effect_id=1):
    #play selected effect
    effect_id = drv_set_effect(effect_id)
    drv.sequence[0] = adafruit_drv2605.Effect(effect_id)
    drv.play()
    time.sleep(0.5)
    drv.stop()

def drv_set_run(motor_type=1, effect_id=1):
    #set motor_type
    drv_set_motor(motor_type)
    #set and run effect
    drv_effect_run(effect_id)


### --- TEST/DEBUG ---
def drv_test():
    print("*** DRV2605: TEST ***")
    print("ERM test")
    drv_erm_run()
    print("LRA test")
    drv_lrm_run()

### --- RUN ---
print("DRV2605 - haptic driver")
#while True:
#	drv_run()
