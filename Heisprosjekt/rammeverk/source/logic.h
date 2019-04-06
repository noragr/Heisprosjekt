#include "channels.h"
#include "elev.h"
#include "io.h"
#include "statemachine.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

//Initialize 
int initializer();

int queue[5][N_FLOORS];

int get_order(int floor, elev_motor_direction_t dir);

elev_motor_direction_t get_direction(int pos, elev_motor_direction_t dir);

void set_order(int pos, int floor, elev_button_type_t button);

void delete_order(int floor);

