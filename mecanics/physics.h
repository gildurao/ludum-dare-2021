/* All physics related code will be placed here */

#include <stdio.h>

extern void colision();
extern uint8_t over_ground();
extern uint8_t y_collision();
extern uint8_t walls_right();
extern uint8_t walls_left();
extern uint8_t walls_down();
extern uint8_t walls_up();
extern uint8_t walls_down_check();


#define DEFAULT_X_SPEED 1
#define MAX_X_SPEED 300
#define MAX_Y_SPEED 500
#define GRAVITY 9
