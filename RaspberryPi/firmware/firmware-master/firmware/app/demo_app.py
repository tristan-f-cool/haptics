#*** Precision Microdrives [PMD] ***
#* Haptic Feedback Evaluation Kit 2.0 [HFEK2]
#- Tristan Cool
#- July 2019

#********************************************************#
#    This script runs the touchscreen-click demo app	 #
#********************************************************#

### --- TITLE ---
print("............. -PMD HFEK2- TOUCHSCREEN DEMO APP_1 (CLICK) ..................")
print("demo_app.py")

### --- IMPORT ---
import pygame
import os
#import pygameui as ui
import logging
import RPi.GPIO as GPIO

import sys
sys.path.insert(1,'/home/pi/firmware/firmware')
import drv2605 as drv

sys.path.insert(1,'/home/pi/pygameui')
import pygameui as ui

### --- SYSTEM ---
## - GPIO setup
GPIO.setmode(GPIO.BCM)
#GPIO.setup(4, GPIO.OUT)

## - DRV2605 haptic 
drv.drv_set_motor(1)

## - Format: console, logger, handler
log_format = '%(asctime)-6s: %(name)s - %(levelname)s - %(message)s'
console_handler = logging.StreamHandler()
console_handler.setFormatter(logging.Formatter(log_format))
logger = logging.getLogger()
logger.setLevel(logging.DEBUG)
logger.addHandler(console_handler)

## - Format: OS, mouse, touchscreen
os.putenv('SDL_FBDEV', '/dev/fb1')
os.putenv('SDL_MOUSEDRV', 'TSLIB')
os.putenv('SDL_MOUSEDEV', '/dev/input/touchscreen')

### --- GUI ---
MARGIN = 20

### --- CLASS ---
class PiTft(ui.Scene):
    def __init__(self):
        ui.Scene.__init__(self)

    #Double-click button
    self.motor_button = ui.Button(ui.Rect(MARGIN, MARGIN, 130, 90), 'DOUBLE')
    self.motor_button.on_clicked.connect(self.gpi_button)
    self.add_child(self.motor_button)

    #Single-click button
    self.pmd_button = ui.Button(ui.Rect(170, MARGIN, 130, 90), 'SINGLE')
    self.pmd_button.on_clicked.connect(self.gpi_button)
    self.add_child(self.pmd_button)

    #Back button
    self.back_button = ui.Button(ui.Rect(MARGIN, 130, 130, 90), '<--')
    self.back_button.on_clicked.connect(self.gpi_button)
    self.add_child(self.back_button)

    #Play button
    self.play_button = ui.Button(ui.Rect(170, 130, 130, 90), 'PLAY')
    self.play_button.on_clicked.connect(self.gpi_button)
    self.add_child(self.play_button)

    #Haptic effects
    effects = (5,25,35,36)
    current_effect = effect[0]

    def gpi_button(self, btn, mbtn):
        logger.info(btn.text)

        if btn.text == 'DOUBLE':
            #print("double click")
            if current_effect in [5,25,35]:
                current_effect = effects[3]
            elif current_effect in [5,25,36]:
                 current_effect = effects[2]
            drv.drv_effect_run(current_effect)
        elif btn.text == 'SINGLE':
            #print("single click")
            if current_effect in [5,35,36]:
                current_effect = effects[1]
            elif current_effect in [25,35,36]:
                current_effect = effects[0]
            drv.drv_effect_run(current_effect)
        elif btn.text == '<--':
            #print("BACK")
            drv.drv_effect_run(effects[0])
            pygame.quit()
        elif btn.text == 'PLAY':
            #print("PLAY")
            drv.drv_effect_run(current_effect)

## - UI / RUN
ui.init('PMD HFEK2 Touchscreen Demo', (320, 240))
pygame.mouse.set_visible(False)
ui.scene.push(PiTft())
ui.run()
