###################################################################
#	Make file for PhilipMallory_FinalProject.						  
#																  
#	NOTE: Do not change this file unless otherwise instructed.    
#																  
###################################################################

DIR = /cygdrive/c/gba_studio
include $(DIR)/system/master.mak



 ####################
# GBA ROM Image Name #
 ####################

PROGNAME="PhilipMallory_FinalProject"



 #########################################
# Set a list of files you want to compile #
 #########################################
 
OFILES += basicPalette.o battleBG.o battleFG.o battleLoop.o defeat.o game_arena.o game_overworld.o instructionsImg.o lose.o main.o myLib.o overworld_map.o pause.o pauseIMG.o shootNoise.o soundLoop.o splash.o splashImg.o stats_screen.o victory.o walking_sprite.o win.o



 #########################################################
# Compile using Krawall (sound stuff, don't mess with it) #
 #########################################################
 
USE_KRAWALL=no
KRAWALL_IS_REGISTERED=no
KRAWALL_FILES=



 ######################################
# Standard Makefile targets start here #
 ######################################
all : $(PROGNAME).$(EXT) clean



 ##############################################
# Most Makefile targets are predefined for you,#
# such as vba, clean ... in the following file #
 ##############################################

include $(DIR)/system/standard-targets.mak



 #####################################
# Custom  Makefile targets start here #
 #####################################

gfx: makefile




