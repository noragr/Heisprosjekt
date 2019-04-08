#ifndef __INCLUDE_STATEMACHINE_H__
#define __INCLUDE_STATEMACHINE_H__


void open_door();

void close_door();

void stop_elev();

void emergency_stop();

//Status
typedef enum /*state_machine_type*/ {
	MOVING,
	INITIALIZED,
	IDLE,
	STOPPED,
	EMERGENCY,
	FAILURE
} state_machine_type_t;

state_machine_type_t state_machine(state_machine_type_t current_state);

#endif // #ifndef __INCLUDE_STATEMACHINE_H__