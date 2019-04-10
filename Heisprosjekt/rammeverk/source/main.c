#include "elev.h"
#include "logic.h"
#include "timer.h"
#include "statemachine.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

    printf("Press obstruction button to stop elevator and exit program.\n");
    time_t start = start_time();
    while (!(timer_expired(start))) {
        printf("Timer Test\n");
    }


    static state_machine_type_t next_state;
    static state_machine_type_t current_state;
    static elev_motor_direction_t current_dir;

    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
    elev_set_motor_direction(DIRN_DOWN);
    printf("satte opp\n");
    while (1) {

        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
            current_dir = DIRN_DOWN;
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
            current_dir = DIRN_UP;
        }

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            current_dir = DIRN_STOP;
            break;
        }
    }

    current_dir = DIRN_STOP;
    elev_set_motor_direction(current_dir);
    current_state = INITIALIZED;
    printf("Klar for statemachine\n");
    

    while (current_state != FAILURE){
        for (int i = 0; i < 3; i ++) {
            for (int f = 0; f < N_FLOORS; f++) {
                printf("%d", queue[i][f]);
                }
            printf("\n");
            }
        printf("\n");

        set_order();
        current_state = state_machine(current_state);

        if (elev_get_stop_signal()) {
            current_state = EMERGENCY;
        }
        
        if (current_state == FAILURE){ //blir dette riktig???
            printf("Unable to initialize elevator hardware!\n");
            next_state = INITIALIZED; //hmmm
        }

         if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
            current_dir = DIRN_DOWN;
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
            current_dir = DIRN_UP;
        }
    }
    return 0;
}


/*
        if (elev_get_obstruction_signal()){ //Obstruction signal
            dir = DIRN_STOP;
            elev_set_motor_direction(DIRN_STOP); //Stop elevator
            break; //Exit program
        } 
*/
         // Stop elevator and exit program if the stop button is pressed






/*
 
#include "elev.h"
#include <stdio.h>
int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
    printf("Press STOP button to stop elevator and exit program.\n");
    elev_set_motor_direction(DIRN_UP);
    while (1) {
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }
        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }
    return 0;
}
*/