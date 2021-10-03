#include <gb/gb.h>
#include <stdio.h>
#include "tiles/title_screen/letters.c"
#include "maps/title_screen/title_map.c"
#include "tiles/follow_the_train/follow_the_train_data.c"
#include "maps/follow_the_train/follow_the_train_map.c"
#include "huGEDriver/huGEDriver.h"

#include "maps/SampleMap.h"
#include "tiles/map/SampleTiles.h"
#include "tiles/player/PlayerTile.h"
#include "animation/player_ani.h"
#include "mecanics/physics.h"
#include "constants.h"

typedef void (*nullfunc_t)(void);

#define camera_max_y ((sample_mapHeight - 18) * 8)
#define camera_max_x ((sample_mapWidth - 20) * 8)

#define MIN(A, B) ((A) < (B) ? (A) : (B))

#define NBSFRAMES 0x02 /* Nb frames for the sprite */

extern const hUGESong_t song_Data;

uint8_t joy;

// current and old positions of the camera in pixels
uint16_t camera_x, camera_y, old_camera_x, old_camera_y;
// current and old position of the map in tiles
uint8_t map_pos_x, map_pos_y, old_map_pos_x, old_map_pos_y;
// redraw flag, indicates that camera position was changed
uint8_t redraw;
static uint8_t time = 0; /* Global "time" value (counter) */
fixed sposx, sposy;      /* Sprite position (fixed point) */
fixed fsposx, fsposy;    /* Future Sprite position (fixed point) */
// fixed Last_sposx, Last_sposy; /* Last sprite position (fixed point) */
int16_t sspx, sspy; /* Sprite speed (fixed point) */
uint8_t sframe = 0; /* Current frame of the sprite */
fixed player_x_speed;
uint8_t player_animation = PLAYER_IDLE;

void animate_player(uint8_t *animation);
void place_sprite();
void colison();

// In game variables

uint8_t start_screen_retries = 3;

/* Colison phisics */

/* Set sprite tiles */
void player_sprite(uint8_t *animation)
{
    uint8_t s;

    s = sframe << 1;
    if (animation == PLAYER_RIGHT)
    {
        set_sprite_tile(0, player_right[s]);
    }
    if (animation == PLAYER_LEFT)
    {
        set_sprite_tile(0, player_left[s]);
    }
    if (animation == PLAYER_IDLE)
    {
        set_sprite_tile(0, player_idle[s]);
    }
    // set_sprite_tile(1, player_right[s + 1]);
}

void animate_player(uint8_t *animation)
{
    uint8_t number_farmes = 0x01;
    if (animation == PLAYER_RIGHT || animation == PLAYER_LEFT)
    {
        number_farmes = 0x03;
    }
    else
    {
        number_farmes = 0x01;
        sframe = 0;
    }

    if ((time & 0x07) == 0)

    {
        sframe++;
        if (sframe == number_farmes)
            sframe = 0;
        player_sprite(animation);
    }
}

void place_sprite()
{

    //speed limit

    if (sspx > MAX_X_SPEED)
    {
        sspx = MAX_X_SPEED;
    }
    if (sspx < -MAX_X_SPEED)
    {
        sspx = -MAX_X_SPEED;
    }
    if (sspy > MAX_X_SPEED)
    {
        sspy = MAX_Y_SPEED;
    }
    if (sspy < -MAX_Y_SPEED)
    {
        sspy = -MAX_Y_SPEED;
    }

    uint16_t fx, fy;
    fx = sposx.w + sspx;
    fy = sposy.w + sspy;
    // printf("%d ", sspy);
    if (sspx > 0)
    {

        if (walls_right(0x00, 0) == 0)
        {

            if (fx / 256 < (20 - 7) * 8)
            {
                sposx.w += sspx;
            }
            else if (camera_x < camera_max_x)
            {
                camera_x++;
                redraw = TRUE;
            }
            else if (fx / 256 < 20 * 8)
            {
                sposx.w += sspx;
            }
        }
        if (walls_right(0x01, 0) == 1)
        {
            // showTitleScreen();
            levelOne();
        }
    }
    else if (sspx < 0)
    {
        if (walls_left(0x00, 0) == 0)
        {
            if (fx / 256 > 7 * 8)
            {
                sposx.w += sspx;
            }
            else if (camera_x > 0)
            {
                camera_x--;
                redraw = TRUE;
            }
            else if (fx / 256 > 8)
            {
                sposx.w += sspx;
            }
        }
        // Kill check
        if (walls_left(0x01, 0) == 1)
        {
            // showTitleScreen();
            levelOne();
        }
    }

    if (walls_down_check() == 0)
    {
        sspy = sspy + GRAVITY;
        // sposy.w += sspy;
    }

    if (sspy > 0)
    {
        // DOWN Collision Check
        if (walls_down(0x00, 0) == 0)
        {

            sposy.w += sspy;
        }
        if (walls_down(0x00, 0) == 1)
        {
            sspy = 0;
        }

        // Kill check
        if (walls_down(0x01, 0) == 1)
        {
            // showTitleScreen();
            levelOne();
        }
    }
    else if (sspy < 0)
    {
        if (walls_up(0x00, 0) == 0)
        {
            // printf("%d ", sspy);
            sposy.w += sspy;
        }
        if (walls_up(0x00, 0) == 1)
        {
            sspy = 0;
        }
    }

    move_sprite(0, sposx.b.h, sposy.b.h);
    // move_sprite(1, sposx.b.h + 8, sposy.b.h);
    /* friction */
    sspx = sspx / 2;
}

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
    disable_interrupts();

    LCDC_REG = LCDCF_OFF | LCDCF_BG8800 | LCDCF_BG9800 | LCDCF_OBJON | LCDCF_BGON;
    /*
   * LCD        = Off
   * BG Chr     = 0x8800
   * BG Bank    = 0x9800
   * OBJ        = 8x8
   * OBJ        = On
   * BG         = On
   */
    // Loads sprites to memory
    set_sprite_data(0x00, 11u, player);
    set_sprite_prop(0, 0x00);
    // set_sprite_prop(1, 0x00);
    sframe = 0;
    sposx.w = 0x1000;
    sposy.w = 0x1000;
    sspx = 0x0000;
    sspy = 0x0050;
    player_sprite(PLAYER_RIGHT);
    place_sprite();
    player_x_speed.w = 200;
    set_bkg_data(0, 5u, tiles);

    map_pos_x = map_pos_y = 0;
    old_map_pos_x = old_map_pos_y = 255;
    set_bkg_submap(map_pos_x, map_pos_y, 20, 18, sample_map, sample_mapWidth);
    DISPLAY_ON;

    camera_x = camera_y = 0;
    old_camera_x = camera_x;
    old_camera_y = camera_y;

    DISPLAY_ON;
    enable_interrupts();
    redraw = FALSE;

    SCX_REG = camera_x;
    SCY_REG = camera_y;
    while (TRUE)
    {
        joy = joypad();

        time++;

        // printf("%d", joy);
        animate_player(player_animation);
        // colision();
        place_sprite();

        player_animation = PLAYER_IDLE;
        // Jump
        if (joy & J_B)

        {
            // printf("%d", walls_down_check());
            if (walls_down_check() == 1)
            {
                // sposy.w = sposy.w - 200;
                sspy = -300;
            }
        }

        // up or down
        // if (joy & J_UP)
        // {
        //     if (camera_y)
        //     {
        //         camera_y--;
        //         redraw = TRUE;
        //     }
        // }
        // else if (joy & J_DOWN)
        // {
        //     if (camera_y < camera_max_y)
        //     {
        //         camera_y++;
        //         redraw = TRUE;
        //     }
        // }
        // left or right
        if (joy & J_LEFT)
        {

            player_animation = PLAYER_LEFT;
            sspx = sspx - player_x_speed.w;
        }
        else if (joy & J_RIGHT)
        {

            player_animation = PLAYER_RIGHT;
            sspx = sspx + player_x_speed.w;
        }
        if (redraw)
        {
            wait_vbl_done();

            set_camera();
            redraw = FALSE;
        }
        else
        {
            wait_vbl_done();
        }
    }
}

void showTitleScreen()
{
    __critical
    {
        hUGE_init(&song_Data);
        add_VBL(hUGE_dosound);
    }

    set_bkg_data(0, 11, letters);
    set_bkg_tiles(0, 0, titleMapWidth, titleMapHeight, titleMap);

    SHOW_BKG;
    DISPLAY_ON;

    while (1)
    {
        wait_vbl_done();
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

    delay(3000);

    ((nullfunc_t)0x3000)();
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
    LCDC_REG = 0xD1;
    BGP_REG = 0b11100100;
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;
    showTitleScreen();
}