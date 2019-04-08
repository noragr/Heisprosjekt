#include "elev.h"
#include "logic.h"

#include "statemachine.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main() {
    printf("Press obstruction button to stop elevator and exit program.\n");
    static elev_motor_direction_t dir;
    static state_machine_type_t next_state;
    static state_machine_type_t current_state;

    dir = DIRN_UP;
    elev_set_motor_direction(dir);
    current_state = INITIALIZED;

    while (current_state != FAILURE){
        for (int i = 0; i < 5; i ++) {
        for (int f = 0; f < N_FLOORS; f++) {
            printf("%d", queue[i][f]);
            }
            printf("\n");
        }
        printf("\n");
        set_order();
        current_state = state_machine(current_state);

        /*if (elev_get_obstruction_signal()){ //Obstruction signal
            dir = DIRN_STOP;
            elev_set_motor_direction(DIRN_STOP); //Stop elevator
            break; //Exit program
        } */ 
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            dir = DIRN_STOP;
            break;
        }

         // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            dir = DIRN_STOP;
            break;
        }

        if (current_state == FAILURE){ //blir dette riktig???
            printf("Unable to initialize elevator hardware!\n");
            next_state = INITIALIZED; //hmmm
        }


         if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
            dir = DIRN_DOWN;
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
            dir = DIRN_UP;
        }
    }


    return 0;
}






/*
    elev_motor_direction_t dir;
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");


    int floor = initializer();
    printf("%d\n",floor);
    elev_set_motor_direction(DIRN_UP);

    
    for (int i = 0; i < 5; i ++) {
        for (int f = 0; f < N_FLOORS; f++) {
            printf("%d", queue[i][f]);
        }
        printf("\n");
    }
    printf("\n");





    while (1) {
        

       

        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
            dir = DIRN_DOWN;
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
            dir = DIRN_UP;
        }

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            dir = DIRN_STOP;
            break;
        }
    }

    return 0;
*/