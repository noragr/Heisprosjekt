#include "logic.h"

int initializer(){
	int floor = elev_get_floor_sensor_signal();
	printf("%d\n", floor);

	if (floor == -1){ //not defined floor
		while (floor == -1){
			elev_set_motor_direction(DIRN_DOWN);
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

int get_order(int floor, elev_motor_direction_t dir){
	if (dir == DIRN_UP) {// direction up
		for (int i = floor+1; i < N_FLOORS; i++) {
			if (queue[i][0] == 1 || queue[i][1]==1) {
				return i;
			}
		}
	} else if (dir == DIRN_DOWN){ // direction down 
		for (int i = floor-1; i > -1; i--) {
			if (queue[i][0] == 1 || queue[i][1] == 1) {
				return i;
			}
		}
	}
	return -1; // No orders to manage. 
}

elev_motor_direction_t get_direction(int pos, elev_motor_direction_t dir) {
	if (get_order(pos, dir) > pos) {
		return DIRN_UP;
	}else if (get_order(pos, dir) < pos) {
		return DIRN_DOWN;
	}
	return DIRN_STOP;
}


void set_order(int pos, int floor, elev_button_type_t button){ // tar inn en etasje og retning. 
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
			open_door(); //open door one more time
		}
		queue[4][floor] = 1;
	}
	elev_set_button_lamp(button, floor, 1); //set light in every made orders
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