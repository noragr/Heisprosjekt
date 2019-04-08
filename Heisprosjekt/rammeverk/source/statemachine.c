#include "statemachine.h"
#include "channels.h"
#include "elev.h"
#include "io.h"
#include "logic.h"

#include <stdio.h>

static int current_floor;
static elev_motor_direction_t dir;


//Open door
void open_door(){
	elev_set_door_open_lamp(1);
}

//Close door
void close_door(){
	elev_set_door_open_lamp(0);
}

//Stop elevator
void stop_elev(){
	elev_set_motor_direction(DIRN_STOP);
}

//Emergency button
void emergency_stop(){
	stop_elev();
	elev_set_stop_lamp(1);
	for (int floor = 0; floor < N_FLOORS; floor++){
		delete_order(floor); //delete all orders
	}
	if (elev_get_floor_sensor_signal()!=-1){ //at a floor
		while(elev_get_stop_signal()){
			open_door();
		}
		//TIMER 3 SEC
		close_door();
	}
	else{
		while (elev_get_stop_signal()){
			;
			/*if (elev_get_floor_sensor_signal() || elev_get_button_signal(button, floor)){
				queue[][floor]=0; //ignore new orders
			}*/
		}
	}
}


state_machine_type_t state_machine(state_machine_type_t current_state){
	state_machine_type_t next_state = current_state;
	switch (current_state){
		case INITIALIZED:
		    if (!elev_init()) {
       			printf("Unable to initialize elevator hardware!\n");
       			next_state = INITIALIZED;
       			break;
    		}

			current_floor = initializer();
			next_state = IDLE; //hmmm
			break;

		case MOVING:
			set_order();
			// sjekk om den er fremme
			if (check_order_complete()) {
				next_state = STOPPED;
			}

			// check which floor, set floor_indicator();
			
			if(elev_get_stop_signal()){
				next_state = STOPPED;
				break;
			}else {
				if (elev_get_floor_sensor_signal() != -1) {
					current_floor = elev_get_floor_sensor_signal();
					printf("%d\n", current_floor);
				}
				else{
					next_state = MOVING;
				}
				
			}
			break;

		case IDLE:
			dir = DIRN_STOP;
			current_floor = elev_get_floor_sensor_signal();
			printf("%d\n", current_floor);
			set_order();
			if(elev_get_stop_signal()){
				next_state = STOPPED;
				break;
			}
			if (order_amount() == 0) {
				printf("order_amount er 0\n");
				elev_set_motor_direction(dir);
				next_state = IDLE;
			}else {
				dir = get_direction();
				printf("Hei\n" );
				elev_set_motor_direction(dir);
			}
			break;

		case STOPPED:  // I en etasje!! 
			stop_elev();
			dir = DIRN_STOP;
			elev_set_motor_direction(dir);
			current_floor = elev_get_floor_sensor_signal();
			delete_order(current_floor);
			/*while (timer) {
				stop_elev();
				open_door();
				set_order();
				}*/
			close_door();
			next_state = IDLE;
			
			if(elev_get_stop_signal()){
				next_state = EMERGENCY;
			}
			break;

		case EMERGENCY:
			emergency_stop();
			next_state = IDLE;
			break;

		case FAILURE:
			next_state = FAILURE;
			break;

		default:
			next_state = INITIALIZED;
			break;
	}
	return next_state;
}