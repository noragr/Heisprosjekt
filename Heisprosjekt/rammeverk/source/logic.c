#include "logic.h"
#include "channels.h"
#include "elev.h"
#include "io.h"
#include "statemachine.h"
//#include "timer.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>




static int current_floor;
//static elev_motor_direction_t dir;

int initializer(){

	int floor = elev_get_floor_sensor_signal();
	printf("%d\n", floor);

	if (floor == -1){ //not defined floor
		while (floor == -1){
			printf("ikke initialisert\n");
			elev_set_motor_direction(DIRN_UP);
			floor = elev_get_floor_sensor_signal();
		}
		printf("initialisert\n");
		elev_set_motor_direction(DIRN_STOP);	
	}
	elev_set_floor_indicator(floor);
	return floor;
}
 //Queue matrix
int queue[3][N_FLOORS] = {
	{0, 0, 0, 0} , //lamp: BUTTON_CALL_UP
	{0, 0, 0, 0} , //lamp: BUTTON_CALL_DOWN
	{0, 0, 0, 0} //lamp: BUTTON_COMMAND (inside elevator)
};

int get_order(int floor, elev_button_type_t button) {
	return queue[button][floor];
}


elev_motor_direction_t get_direction(elev_motor_direction_t dir) {

	current_floor = elev_get_floor_sensor_signal();
	if (current_floor != -1) {
		elev_set_floor_indicator(current_floor);
	}

	if (order_amount() != 0) {
		// algoritme for å velge retning 
		if ((get_order(current_floor, 0) || get_order(current_floor, 1) || get_order(current_floor, 2)) && elev_get_floor_sensor_signal() != -1) {  // allerede i etasjen 
			open_door();
			/*clock_t start_value = start_timer();
			while(!(timer_expired(start_value))) {
					set_order();
					printf("dør åpnet\n");
			}*/
			close_door();
			printf("dør lukket\n");
		}  
		else if (elev_get_floor_sensor_signal() == -1 &&  ((order_is_in_dir(dir)==-1*dir) && dir != 0)) {     // om den er stuck 
			dir = -1*dir;

		}else if (order_is_in_dir(dir)) { // om den er i riktig retning
			return dir;

		}  else if (order_is_in_dir(dir) == -1) {   // om den er i feil retning 
			dir = -1*dir;

		} else if (dir == DIRN_STOP) {  // heisen står stille . 
			for (int floor = current_floor + 1; floor < N_FLOORS; floor++) {
				for (elev_button_type_t button = 0; button < 3; button++) {
					if (get_order(floor, button)) {
						dir = DIRN_UP;
					}
				}
			}
			if (dir != DIRN_UP) {
				for (int floor = current_floor - 1; floor >= 0; floor--) {
					for (elev_button_type_t button = 0; button < 3; button++) {
						if (get_order(floor, button)) {
							dir = DIRN_DOWN;
						}
					}
				}
			}
		}

	} else {
		if (elev_get_floor_sensor_signal() == -1) {  // no orders and elevator between floors. 
			current_floor = -1;
			current_floor = initializer();
			elev_set_floor_indicator(current_floor);
			dir = DIRN_STOP;
		}
	}
	return dir;
}


void set_order(){  // tar inn en etasje og retning.  // husk å endre!!!
	current_floor = elev_get_floor_sensor_signal();
	for (int floor = 0; floor < N_FLOORS; floor++) {
		if (elev_get_button_signal(BUTTON_COMMAND, floor)) {
			queue[2][floor] = 1;
			elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
		}
	}
	for (int floor = 0; floor < N_FLOORS-1; floor++) {
		if (elev_get_button_signal(BUTTON_CALL_UP, floor)) {
			queue[0][floor] = 1;
			elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
		}
	}
	for (int floor = 1; floor < N_FLOORS; floor++) {
		if (elev_get_button_signal(BUTTON_CALL_DOWN, floor)) {
			queue[1][floor] = 1;
			elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
		}
	}

}


void delete_order(int floor){
	// fjerne lys 
	for (int lys = 0; lys < 3; lys++) {
		if (queue[lys][floor] == 1){
			elev_set_button_lamp(lys, floor, 0);
		}
	}

	for (int i = 0; i < 3; i++) {
		queue[i][floor] = 0;
	}
	// fjerne lys. 
}


int order_amount() {
	int amount = 0;
	for (int i = 0; i < N_FLOORS; i++) {
		if (queue[0][i] == 1) {    // sjekker bestillinger oppover
			amount++;
		}
		if (queue[1][i] == 1) { // sjekker bestillinger nedover 
			amount++;
		}
	}
	return amount;
}



int order_is_in_dir(elev_motor_direction_t dir) {
	if (dir == 1 || dir == 0) {  // next order is in direction 
		for (int floor = current_floor+1; floor < N_FLOORS; floor++) {
			for (int button = 0; button < 3; button++) {
				if (get_order(floor, button) == 1) {
					return 1;
				}
			}
		}
	}else if (dir == -1 || dir == 0) {  // next order is in opposite direction 
		for (int floor = current_floor-1; floor >= 0; floor--) {
			for (int button = 0; button < 3; button++) {
				if (get_order(floor, button) == 1) {
					return 1; 
				}
			}
		}
	}
	return -1;
}

int check_order_complete(){
	current_floor = elev_get_floor_sensor_signal();
	if (current_floor != -1) {
		elev_set_floor_indicator(current_floor);
	}
	for (int button = 0; button < 3; button++) {
		if (get_order(current_floor, button)) {
			return 1;		}
	}
	return 0;
}

/*
elev_motor_direction_t get_direction(elev_motor_direction_t dir) {

	current_floor = elev_get_floor_sensor_signal();
	if (current_floor != -1) {
		elev_set_floor_indicator(current_floor);
	}

	if (order_amount() != 0) {
		// algoritme for å velge retning 
		if ((get_order(current_floor, 0) || get_order(current_floor, 1) || get_order(current_floor, 2)) && elev_get_floor_sensor_signal() != -1) {  // allerede i etasjen 
			open_door();
			clock_t start_value = start_timer();
			while(!(timer_expired(start_value))) {
					set_order();
					printf("dør åpnet\n");
			}
			close_door();
			printf("dør lukket\n");
		}  
		else if (elev_get_floor_sensor_signal() == -1 &&  ((order_is_in_dir(dir)==-1*dir) && dir != 0)) {     // om den er stuck 
			dir = -1*dir;

		}else if (order_is_in_dir(dir)) { // om den er i riktig retning
			return dir;

		}  else if (order_is_in_dir(dir) == -1) {   // om den er i feil retning 
			dir = -1*dir;

		} else if (dir == DIRN_STOP) {  // heisen står stille . 
			for (int floor = current_floor + 1; floor < N_FLOORS; floor++) {
				for (elev_button_type_t button = 0; button < 3; button++) {
					if (get_order(floor, button)) {
						dir = DIRN_UP;
					}
				}
			}
			if (dir != DIRN_UP) {
				for (int floor = current_floor - 1; floor >= 0; floor--) {
					for (elev_button_type_t button = 0; button < 3; button++) {
						if (get_order(floor, button)) {
							dir = DIRN_DOWN;
						}
					}
				}
			}
		}

	} else {
		if (elev_get_floor_sensor_signal() == -1) {  // no orders and elevator between floors. 
			current_floor = -1;
			current_floor = initializer();
			elev_set_floor_indicator(current_floor);
			dir = DIRN_STOP;
		}
	}
	return dir;
}
*/