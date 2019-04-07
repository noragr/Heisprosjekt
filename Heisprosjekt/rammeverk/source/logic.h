#ifndef __INCLUDE_LOGIC_H__
#define __INCLUDE_LOGIC_H__
//#include "statemachine.h"



int current_floor;
elev_motor_direction_t dir;


//Initialize 
int initializer();

int queue[5][N_FLOORS];

int get_order(int floor, elev_button_type_t button);

elev_motor_direction_t get_direction();

void set_order(int pos, int floor, elev_button_type_t button);

void delete_order(int floor);

int order_amount();

int order_is_in_dir();


#endif // #ifndef __INCLUDE _LOGIC_H__