#include "elev.h"
#include "logic.c"
#include "statemachine.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main() {
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
        int a = 1;
        int floor = elev_get_floor_sensor_signal();
        elev_button_type_t button;
        for (int f = 0; f < N_FLOORS; ++f) {
            if (elev_get_button_signal(BUTTON_CALL_UP, f) == 1) {
                button = BUTTON_CALL_UP;
                printf("UP\n");
                set_order(floor, f, button);

            }else if (elev_get_button_signal(BUTTON_CALL_DOWN, f) == 1) {
                button = BUTTON_CALL_DOWN;
                printf( "DOWN\n");
                set_order(floor, f, button);

            }else if (elev_get_button_signal(BUTTON_COMMAND, f) == 1) {
                button = BUTTON_COMMAND;
                printf("COMMAND\n");
                set_order(floor, f, button);
            }
            //dette er bare en test
        }

        for (int i = 0; i < 5; i++) {
                int time_start = clock();
                while (clock() < time_start + 10000) {
                    a =  1;
                }
            }
        
        delete_order(2);
        int bestilling = get_order(floor, dir);
        if (!(bestilling == -1)) {
            printf("%d\n", bestilling);
            dir = get_direction(bestilling, dir);
            printf("%d\n", dir);
            
        }
        
        /*
        dir = get_direction(bestilling, dir);
        while (!(elev_get_floor_sensor_signal() == bestilling)) {
            elev_set_motor_direction(dir);
            printf("%d\n", dir);
            if (elev_get_floor_sensor_signal() == bestilling) {
                printf("FRAMME!!\n");
            }
        }*/



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
}



   /*for (int i = 0; i < N_FLOORS; i++) {
            for (int j = 0; j < 3; j++) {
                if (!(elev_get_button_signal(j, i) == 0)) {
                    set_order(elev_get_floor_sensor_signal(), i, j);
                    int floor = get_order(elev_get_floor_sensor_signal(), dir);
                    dir = get_direction(elev_get_floor_sensor_signal(), dir);
                    while (!(elev_get_floor_sensor_signal() == floor)) {
                        elev_set_motor_direction(dir);
                    }
                    elev_set_motor_direction(DIRN_STOP);
                    dir = DIRN_STOP;
                }
            }
        }
        int floor = elev_get_floor_sensor_signal();
        */
