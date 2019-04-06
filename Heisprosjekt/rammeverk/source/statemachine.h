

void open_door();

void close_door();

void stop_elev();

void emergency_stop();

//Status
typedef enum state_machine_type {
	MOVING,
	IDLE,
	STOPPED,
	EMERGENCY
} state_machine_type_t;