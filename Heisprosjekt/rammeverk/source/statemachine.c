#include "statemachine.h"
#include "channels.h"
#include "elev.h"
#include "io.h"
#include "logic.h"

#include <stdio.h>

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


