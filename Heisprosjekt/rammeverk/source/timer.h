#include <time.h>
#include <stdio.h>

time_t start_value;
time_t end_value;

void start_timer(); //starts the timer

//Check if timer has expired
//Returns 1 if it has and 0 otherwise
int timer_expired(double seconds);