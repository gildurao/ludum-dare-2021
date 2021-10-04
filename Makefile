#
# Simple Makefile that compiles all .c and .s files in the same folder
#

# If you move this project you can change the directory 
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
GBDK_HOME = ../../

LCC = $(GBDK_HOME)bin/lcc 

# You can uncomment the line below to turn on debug output
# LCC = $(LCC) -debug

# You can set the name of the .gb ROM file here
PROJECTNAME    = unSTabLe

BINS	    = bin/$(PROJECTNAME).gb
MAP = maps/SampleMap.c
TILES = tiles/map/SampleTiles.c  tiles/player/PlayerTile.c
ANIMATION = animation/player_ani.c
MECANICS = mecanics/physics.c
BGMUSIC = bg_music/hugetracker_bg_music.c
HUGEDRIVER = huGEDriver/hUGEDriver.obj.o
CSOURCES   := $(wildcard *.c)
ASMSOURCES := $(wildcard *.s)

all:	$(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | grep -v make >> compile.bat

# Compile and link all source files in a single call to LCC
$(BINS):	$(CSOURCES) $(ASMSOURCES)
	$(LCC) -o $@ $(CSOURCES) $(MAP) $(TILES) $(ANIMATION) $(MECANICS) $(HUGEDRIVER) $(BGMUSIC) $(ASMSOURCES)

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm

