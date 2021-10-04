/* All physics related code will be placed here */

#include <stdio.h>

extern void colision();
extern uint8_t over_ground();
extern uint8_t y_collision();
extern uint8_t walls_right(uint8_t start_sprite, uint8_t number_srpites);
extern uint8_t walls_left(uint8_t start_sprite, uint8_t number_srpites);
extern uint8_t walls_down(uint8_t start_sprite, uint8_t number_srpites);
extern uint8_t walls_up(uint8_t start_sprite, uint8_t number_srpites);
extern uint8_t walls_down_check(uint8_t start_sprite, uint8_t number_srpites);
extern uint8_t die_right(uint8_t start_sprite, uint8_t number_srpites);

#define DEFAULT_X_SPEED 1
#define MAX_X_SPEED 300
#define MAX_Y_SPEED 500
#define GRAVITY 9
