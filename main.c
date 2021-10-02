#include <gb/gb.h>
#include <stdio.h>
#include "tiles/title_screen/letters.c"
#include "maps/title_screen/title_map.c"

void main(void)
{
    puts("Hello World");
}

void showTitleScreen()
{
    set_bkg_data(0, 11, letters);
    set_bkg_tiles(0, 0, titleMapWidth, titleMapHeight, titleMap);

    SHOW_BKG;
    DISPLAY_ON;

    while (1)
    {
        scroll_bkg(1, 1);
        delay(100);
    }
}