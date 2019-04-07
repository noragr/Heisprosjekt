#ifndef __INCLUDE_LOGIC_H__
#define __INCLUDE_LOGIC_H__
//#include "statemachine.h"
#include "elev.h"




//Initialize 
int initializer();

int queue[5][N_FLOORS];

int get_order(int floor, elev_button_type_t button);

elev_motor_direction_t get_direction();

void set_order();  

void delete_order(int floor);

int order_amount();

int order_is_in_dir();

int check_order_complete(); //return 1 if complete, 0 otherwise


#endif // #ifndef __INCLUDE _LOGIC_H__