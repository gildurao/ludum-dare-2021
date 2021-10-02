#include <gb/gb.h>
#include <stdio.h>
#include "tiles/title_screen/letters.c"
#include "maps/title_screen/title_map.c"
#include "tiles/follow_the_train/follow_the_train_data.c"
#include "maps/follow_the_train/follow_the_train_map.c"

#include "maps/SampleMap.h"
#include "tiles/SampleTiles.h"

#include "constants.h"

#define camera_max_y ((sample_mapHeight - 18) * 8)
#define camera_max_x ((sample_mapWidth - 20) * 8)

#define MIN(A, B) ((A) < (B) ? (A) : (B))

uint8_t joy;

// current and old positions of the camera in pixels
uint16_t camera_x, camera_y, old_camera_x, old_camera_y;
// current and old position of the map in tiles
uint8_t map_pos_x, map_pos_y, old_map_pos_x, old_map_pos_y;
// redraw flag, indicates that camera position was changed
uint8_t redraw;

// In game variables

uint8_t start_screen_retries = 3;

void set_camera()
{
    // update hardware scroll position
    SCY_REG = camera_y;
    SCX_REG = camera_x;
    // up or down
    map_pos_y = (uint8_t)(camera_y >> 3u);
    if (map_pos_y != old_map_pos_y)
    {
        if (camera_y < old_camera_y)
        {
            set_bkg_submap(map_pos_x, map_pos_y, MIN(21u, sample_mapWidth - map_pos_x), 1, sample_map, sample_mapWidth);
        }
        else
        {
            if ((sample_mapHeight - 18u) > map_pos_y)
                set_bkg_submap(map_pos_x, map_pos_y + 18u, MIN(21u, sample_mapWidth - map_pos_x), 1, sample_map, sample_mapWidth);
        }
        old_map_pos_y = map_pos_y;
    }
    // left or right
    map_pos_x = (uint8_t)(camera_x >> 3u);
    if (map_pos_x != old_map_pos_x)
    {
        if (camera_x < old_camera_x)
        {
            set_bkg_submap(map_pos_x, map_pos_y, 1, MIN(19u, sample_mapHeight - map_pos_y), sample_map, sample_mapWidth);
        }
        else
        {
            if ((sample_mapWidth - 20u) > map_pos_x)
                set_bkg_submap(map_pos_x + 20u, map_pos_y, 1, MIN(19u, sample_mapHeight - map_pos_y), sample_map, sample_mapWidth);
        }
        old_map_pos_x = map_pos_x;
    }
    // set old camera position to current camera position
    old_camera_x = camera_x, old_camera_y = camera_y;
}

void levelOne(void)
{
    DISPLAY_OFF;
    SHOW_BKG;
    set_bkg_data(0, 20u, tiles);

    map_pos_x = map_pos_y = 0;
    old_map_pos_x = old_map_pos_y = 255;
    set_bkg_submap(map_pos_x, map_pos_y, 20, 18, sample_map, sample_mapWidth);
    DISPLAY_ON;

    camera_x = camera_y = 0;
    old_camera_x = camera_x;
    old_camera_y = camera_y;

    redraw = FALSE;

    SCX_REG = camera_x;
    SCY_REG = camera_y;
    while (TRUE)
    {
        joy = joypad();

        // printf("%d", joy);

        // up or down
        if (joy & J_UP)
        {
            if (camera_y)
            {
                camera_y--;
                redraw = TRUE;
            }
        }
        else if (joy & J_DOWN)
        {
            if (camera_y < camera_max_y)
            {
                camera_y++;
                redraw = TRUE;
            }
        }
        // left or right
        if (joy & J_LEFT)
        {
            if (camera_x)
            {
                camera_x--;
                redraw = TRUE;
            }
        }
        else if (joy & J_RIGHT)
        {
            if (camera_x < camera_max_x)
            {
                camera_x++;
                redraw = TRUE;
            }
        }
        if (redraw)
        {
            wait_vbl_done();
            set_camera();
            redraw = FALSE;
        }
        else
            wait_vbl_done();
    }
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
        check_player_start_activity();
    }
}

void show_all_you_had_to_do_was_press_the_start_button_screen()
{
    move_bkg(0, 0);

    set_bkg_data(0, 121, follow_the_train_data);
    set_bkg_tiles(0, 0, 20, 18, follow_the_train_map);

    SHOW_BKG;

    exit();
}

void check_player_start_activity()
{

    if (start_screen_retries > 0)
    {
        joy = joypad();

        if (joy != 0)
        {
            if (joy != J_START)
            {
                show_all_you_had_to_do_was_press_the_start_button_screen();
            }
            else
            {
                start_screen_retries--;
            }
        }
    }
    else
    {
        levelOne();
    }
}

void main(void)
{
    showTitleScreen();
}