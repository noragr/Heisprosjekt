#ifndef __INCLUDE_STATEMACHINE_H__
#define __INCLUDE_STATEMACHINE_H__




void open_door();

void close_door();

void stop_elev();

void emergency_stop();

//Status
typedef enum /*state_machine_type*/ {
	MOVING,
	IDLE,
	STOPPED,
	EMERGENCY
} state_machine_type_t;


#endif // #ifndef __INCLUDE_STATEMACHINE_H__