#include <gb/gb.h>
#include <gb/drawing.h>
#include <stdio.h>
#include "../maps/SampleMap.h"

extern fixed sposx, sposy; /* Sprite position (fixed point) */
extern int16_t sspx, sspy; /* Sprite speed (fixed point) */
extern uint16_t camera_x, camera_y, old_camera_x, old_camera_y;

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

uint8_t walls_right(uint8_t start_sprite, uint8_t number_srpites)
{
    uint16_t fx, fy;
    fx = sposx.w + sspx;
    fy = sposy.w + sspy;
    uint16_t player_tile_x = (fx / 256 - 1 + camera_x) / 8;
    uint16_t player_tile_y_low = (fy / 256 - 10) / 8;
    uint16_t player_tile_y_high = (fy / 256 - 15) / 8;
    uint16_t tile_index_right_low = sample_mapWidth * (player_tile_y_low) + player_tile_x;
    uint16_t tile_index_right_high = sample_mapWidth * (player_tile_y_high) + player_tile_x;
    if (sample_map[tile_index_right_low] >= start_sprite && sample_map[tile_index_right_low] <= start_sprite + number_srpites || sample_map[tile_index_right_high] >= start_sprite && sample_map[tile_index_right_high] <= start_sprite + number_srpites)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t walls_left(uint8_t start_sprite, uint8_t number_srpites)
{

    uint16_t fx, fy;
    fx = sposx.w + sspx;
    fy = sposy.w + sspy;
    uint16_t player_tile_x = (fx / 256 + camera_x - 7) / 8;
    uint16_t player_tile_y_low = (fy / 256 - 10) / 8;
    uint16_t player_tile_y_high = (fy / 256 - 15) / 8;
    uint16_t tile_index_right_low = sample_mapWidth * (player_tile_y_low) + player_tile_x;
    uint16_t tile_index_right_high = sample_mapWidth * (player_tile_y_high) + player_tile_x;
    if (sample_map[tile_index_right_low] >= start_sprite && sample_map[tile_index_right_low] <= start_sprite + number_srpites || sample_map[tile_index_right_high] >= start_sprite && sample_map[tile_index_right_high] <= start_sprite + number_srpites)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t walls_down(uint8_t start_sprite, uint8_t number_srpites)
{

    uint16_t fx, fy;
    fx = sposx.w + sspx;
    fy = sposy.w + sspy;
    uint16_t player_tile_x_left = (fx / 256 - 6 + camera_x) / 8;
    uint16_t player_tile_x_right = (fx / 256 - 3 + camera_x) / 8;
    uint16_t player_tile_y = (fy / 256 - 9) / 8;
    uint16_t tile_index_left = sample_mapWidth * (player_tile_y) + player_tile_x_left;
    uint16_t tile_index_right = sample_mapWidth * (player_tile_y) + player_tile_x_right;

    if (sample_map[tile_index_left] >= start_sprite && sample_map[tile_index_left] <= start_sprite + number_srpites || sample_map[tile_index_right] >= start_sprite && sample_map[tile_index_right] <= start_sprite + number_srpites)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t walls_down_check(uint8_t start_sprite, uint8_t number_srpites)
{

    uint16_t player_tile_x_left = (sposx.b.h - 6 + camera_x) / 8;
    uint16_t player_tile_x_right = (sposx.b.h - 3 + camera_x) / 8;
    uint16_t player_tile_y = (sposy.b.h - 8) / 8;
    uint16_t tile_index_left = sample_mapWidth * (player_tile_y) + player_tile_x_left;
    uint16_t tile_index_right = sample_mapWidth * (player_tile_y) + player_tile_x_right;

    if (sample_map[tile_index_left] >= start_sprite && sample_map[tile_index_left] <= start_sprite + number_srpites || sample_map[tile_index_right] >= start_sprite && sample_map[tile_index_right] <= start_sprite + number_srpites)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t walls_up(uint8_t start_sprite, uint8_t number_srpites)
{
    uint16_t fx, fy;
    fx = sposx.w + sspx;
    fy = sposy.w + sspy;
    uint16_t player_tile_x_left = (fx / 256 - 8 + camera_x) / 8;
    uint16_t player_tile_x_right = (fx / 256 - 2 + camera_x) / 8;
    uint16_t player_tile_y = (fy / 256 - 16) / 8;
    uint16_t tile_index_left = sample_mapWidth * (player_tile_y) + player_tile_x_left;
    uint16_t tile_index_right = sample_mapWidth * (player_tile_y) + player_tile_x_right;

    if (sample_map[tile_index_left] >= start_sprite && sample_map[tile_index_left] <= start_sprite + number_srpites || sample_map[tile_index_right] >= start_sprite && sample_map[tile_index_right] <= start_sprite + number_srpites)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t die_right(uint8_t start_sprite, uint8_t number_srpites)
{

    uint16_t player_tile_x = (sposx.b.h - 1 + camera_x) / 8;
    uint16_t player_tile_y_low = (sposy.b.h - 10) / 8;
    uint16_t player_tile_y_high = (sposy.b.h - 15) / 8;
    uint16_t tile_index_right_low = sample_mapWidth * (player_tile_y_low) + player_tile_x;
    uint16_t tile_index_right_high = sample_mapWidth * (player_tile_y_high) + player_tile_x;
    if (sample_map[tile_index_right_low] >= start_sprite && sample_map[tile_index_right_low] <= start_sprite + number_srpites || sample_map[tile_index_right_high] >= start_sprite && sample_map[tile_index_right_high] <= start_sprite + number_srpites)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}