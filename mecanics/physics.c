#include <gb/gb.h>
#include <stdio.h>
#include "../maps/SampleMap.h"

extern fixed sposx, sposy;           /* Sprite position (fixed point) */
extern fixed Last_sposx, Last_sposy; /* Last sprite position (fixed point) */
extern fixed sspx, sspy;             /* Sprite speed (fixed point) */
extern uint16_t camera_x, camera_y, old_camera_x, old_camera_y;
void colision()
{

    uint16_t player_tile_x = (sposx.b.h - 8) / 8 + (camera_x / 8);
    uint16_t player_tile_y = (sposy.b.h + 1) / 8;
    uint16_t tile_index_under = sample_mapWidth * (player_tile_y - 1) + player_tile_x;
    uint16_t tile_index_over = sample_mapWidth * (player_tile_y - 2) + player_tile_x;
    uint16_t tile_index_right = sample_mapWidth * (player_tile_y) + player_tile_x + 1;
    uint16_t tile_index_left = sample_mapWidth * (player_tile_y) + player_tile_x - 1;

    if (sample_map[tile_index_under] == 0x00)
    {
        sspy.w = 0;
    }
    if (sample_map[tile_index_right] == 0x00)
    {
        sspx.w = 0;
    }
    if (sample_map[tile_index_right] == 0x00)
    {
        sspx.w = 0;
    }
    if (sample_map[tile_index_under] == 0x02)
    {
        sspy.w = sspy.w + 0x005;
    }
}

uint8_t over_ground()
{
    uint16_t player_tile_x = (sposx.b.h - 8) / 8 + (camera_x / 8);
    uint16_t player_tile_y = (sposy.b.h + 1) / 8;
    uint16_t tile_index_under = sample_mapWidth * (player_tile_y - 1) + player_tile_x;
    uint16_t tile_index_over = sample_mapWidth * (player_tile_y - 2) + player_tile_x;
    if (sample_map[tile_index_under] == 0x00 && sample_map[tile_index_over] != 0x00)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t y_collision()
{
    uint16_t player_tile_x = (sposx.b.h - 8) / 8 + (camera_x / 8);
    uint16_t player_tile_y = (sposy.b.h) / 8;
    uint16_t tile_index_under = sample_mapWidth * (player_tile_y - 1) + player_tile_x;
    uint16_t tile_index_over = sample_mapWidth * (player_tile_y - 2) + player_tile_x;
    if (sample_map[tile_index_under] == 0x00 || sample_map[tile_index_over] == 0x00)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t walls_right()
{
    uint16_t player_tile_x = ((sposx.b.h + 1) + camera_x) / 8;
    uint16_t player_tile_y_low = (sposy.b.h - 9) / 8;
    uint16_t player_tile_y_high = (sposy.b.h - 15) / 8;
    uint16_t tile_index_right_low = sample_mapWidth * (player_tile_y_low) + player_tile_x;
    uint16_t tile_index_right_high = sample_mapWidth * (player_tile_y_high) + player_tile_x;
    if (sample_map[tile_index_right_low] == 0x00 || sample_map[tile_index_right_high] == 0x00)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t walls_left()
{
    uint16_t player_tile_x = ((sposx.b.h - 9) + camera_x) / 8;
    uint16_t player_tile_y_low = (sposy.b.h - 9) / 8;
    uint16_t player_tile_y_high = (sposy.b.h - 15) / 8;
    uint16_t tile_index_low = sample_mapWidth * (player_tile_y_low) + player_tile_x;
    uint16_t tile_index_high = sample_mapWidth * (player_tile_y_high) + player_tile_x;
    if (sample_map[tile_index_low] == 0x00 || sample_map[tile_index_high] == 0x00)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}