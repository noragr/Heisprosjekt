#include "logic.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "channels.h"
#include "elev.h"
#include "io.h"
//#include "statemachine.h"


static int current_floor;
static elev_motor_direction_t current_dir;



int initializer(){
	int floor = elev_get_floor_sensor_signal();
	printf("%d\n", floor);

	if (floor == -1){ //not defined floor
		while (floor == -1){
			elev_set_motor_direction(1);
			floor = elev_get_floor_sensor_signal();
		}	
	}
	elev_set_motor_direction(DIRN_STOP);
	elev_set_floor_indicator(floor);
	return floor;
}
 //Queue matrix
int queue[5][N_FLOORS] = {
	{0, 0, 0, 0} , //ordered up
	{0, 0, 0, 0} , //ordered down
	{0, 0, 0, 0} , //lamp: BUTTON_CALL_UP
	{0, 0, 0, 0} , //lamp: BUTTON_CALL_DOWN
	{0, 0, 0, 0} //lamp: BUTTON_COMMAND (inside elevator)
};

int get_order(int floor, elev_button_type_t button) {
	return queue[button+2][floor];
}


elev_motor_direction_t get_direction() {

	if (order_amount() != 0) {
		// algoritme for å velge retning 
		if ((get_order(current_floor, 0) || get_order(current_floor, 1) || get_order(current_floor, 2)) && elev_get_floor_sensor_signal() != -1) {  // allerede i etasjen 
			// start timer. Open door 
			// next_state = ARRIVED
		}  
		else if (elev_get_floor_sensor_signal() == -1 &&  !order_is_in_dir()) {     // om den er stuck 
			dir = -1*dir;

		}else if (order_is_in_dir()) { // om den er i riktig retning

		}  else if (!order_is_in_dir()) {


		} else if (dir == 0) {  // heisen står stille . 
			for (int floor = current_floor + 1; floor < N_FLOORS; floor++) {
				for (elev_button_type_t button = 0; button < 3; button++) {
					if (get_order(floor, button)) {
						dir = 1;
					}
				}
			}for (int floor = current_floor - 1; floor >= 0; floor--) {
				for (elev_button_type_t button = 0; button < 3; button++) {
					if (get_order(floor, button)) {
						dir = -1;
					}
				}
			}
		}

	} else {
		if (elev_get_floor_sensor_signal() == -1) {  // no orders and elevator between floors. 
			current_floor = -1;
			while(current_floor == -1) {  // drive down until you reach a floor. 
				dir = -1;
				current_floor = elev_get_floor_sensor_signal();
			}
			dir = 0;
		}
	}
	return dir;
}


void set_order(){  // tar inn en etasje og retning.  // husk å endre!!!
	for (int floor = 0; floor < N_FLOORS; floor++) {
		for (elev_button_type_t button = 0; button < 3; button++) {
			if (elev_get_button_signal(button, floor)) {
				if (current_floor == floor) {
					elev_set_door_open_lamp(1);
					// timer på 3 sek. 

					delete_order(floor);
				}else {
					queue[button+2][floor] = 1;
					elev_set_button_lamp(button, floor, 1);
				}
			}
		}
	}



	/*
	if (button == BUTTON_CALL_UP) {
		queue[0][floor] = 1;
		queue[2][floor] = 1;
		//elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
		// mer
	}
	else if (button == BUTTON_CALL_DOWN){
		queue[1][floor] = 1;
		queue[3][floor] = 1;
	} 
	if (button == BUTTON_COMMAND) {
		if (pos < floor) { //move upward
			queue[0][floor]=1;
		}else if (pos > floor){ //move down
			queue[1][floor] = 1;
		}else if (pos == floor){ //same floor
			//open_door();
			elev_set_door_open_lamp(1); //open door one more time
		}
		queue[4][floor] = 1;
	}
	elev_set_button_lamp(button, floor, 1); *///set light in every made orders
}


void delete_order(int floor){
	// fjerne lys 
	for (int lys = 2; lys < 5; lys++) {
		if (queue[lys][floor] == 1){
			elev_set_button_lamp(lys-2, floor, 0);
		}
	}

	for (int i = 0; i < 5; i++) {
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



int order_is_in_dir() {
	if (dir == 1) {  // next order is in direction 
		for (int floor = current_floor+1; floor < N_FLOORS; floor++) {
			for (int button = 1; button < 3; button++) {
				if (get_order(floor, button) == 1) {
					return 1;
				}
			}
		}
	}else if (dir == -1) {  // next order is in opposite direction 
		for (int floor = current_floor-1; floor >= 0; floor--) {
			for (int button = 0; button < 3; button++) {
				if (get_order(floor, button) == 1) {
					return -1; 
				}
			}
		}
	}
	return 0;
}

int check_order_complete(){
	for (int button = 0; button < 3; button++) {
		if (get_order(current_floor, button)) {
			return 1;		}
	}
	return 0;
}