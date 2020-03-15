#*** Precision Microdrives [PMD] ***
#* Haptic Feedback Evaluation Kit 2.0 [HFEK2]
#- Tristan Cool
#- September 2019

#********************************************************#
#    This script runs the touchscreen-slider demo app	 #
#********************************************************#

### --- TITLE ---
print("............. -PMD HFEK2- TOUCHSCREEN DEMO APP_2 (SLIDER) ..................")
print("demo_app2.py")

### --- IMPORT ---
import pygame
import os, sys
import math
#import drv2605 as drv

## - Format: OS, mouse, touchscreen
os.putenv('SDL_FBDEV', '/dev/fb1')
os.putenv('SDL_MOUSEDRV', 'TSLIB')
os.putenv('SDL_MOUSEDEV', '/dev/input/touchscreen')

### --- SYSTEM ---
## - DRV2605 haptic
#drv.set_motor(1) #LRA
touch_effect = 5
slide_up_effect = 85
slide_down_effect = 73

### --- GUI ---
black = (0,0,0)
white = (255,255,255)
blue = (0,0,255)
red = (255,0,0)
display_width = 480
display_height = 320

clock = pygame.time.Clock()
gameDisplay = pygame.display.set_mode((display_width,display_height))
pygame.display.set_caption('PMD_DemoApp-Slider')
quit = False

## - PMD logo
imgPMDlogo = pygame.image.load('pi/home/Pictures/PMD_logo2.jpg')
def PMDlogo(x,y):
    gameDisplay.blit(imgPMDlogo, (x,y))
	
x = (display_width * 0.45)
y = (display_height * 0.8)
x_change = 0
y_change = 0
touch_limit = 50
#x_min_limit = x - touch_limit
x_max_limit = x + touch_limit
#y_min_limit = y - touch_limit
y_max_limit = y + touch_limit

## - Slider


### --- FUNCTION ---
while not quit:
    for event in pygame.event.get():
	    #user quit app
        if event.type == pygame.QUIT:
            quit = True
			
	    #touch(mouse)click
	    elif event.type == pygame.MOUSEBUTTONDOWN:
		    print('mouse-touch click')
			#drv.drv_effect_run(touch_effect)
			
	    #touch(mouse)motion
	    elif event.type == pygame.MOUSEMOTION:
		    print('mouse-touch motion')
			#drv.drv_effect_run(touch_effect)
			
		    #logo touched
		    #if event.pos > x_min_limit,y_min_limit and event.pos < x_max_limit,y_max_limit:
			if event.pos < x_max_limit,y_max_limit:
			    print('logo touched')
				#TODO
	        #slider touched
			#TODO
			
    # update logo position	
	x += x_change
	y += y_change
	x_min_limit = x - touch_limit
	x_max_limit = x + touch_limit
	y_min_limit = y - touch_limit
	y_max_limit = y + touch_limit
	
	# update slider position
	#TODO
	
	## update GUI
	gameDisplay.fill(white)
    PMDlogo(x,y)
	pygame.display.flip()
	clock.tick(60)
		
### --- DEBUG ---

### --- RUN ---
