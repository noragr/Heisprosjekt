#include "channels.h"
#include "elev.h"
#include "io.h"

#include <assert.h>
#include <stdlib.h>

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
	//delete all orders
	if (elev_get_floor_sensor_signal()!=-1){ //at a floor
		while(elev_get_stop_signal()){
			open_door();
		}
		//TIMER 3 SEC
		close_door();
	}
	else{
		while (elev_get_stop_signal()){
			//IGNORE ORDERS
		}
	}
}


