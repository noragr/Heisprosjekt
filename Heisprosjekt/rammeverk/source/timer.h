#ifndef __INCLUDE_TIMER_H__
#define __INCLUDE_TIMER_H__




void start_timer(); //starts the timer

//Check if timer has expired
//Returns 1 if it has and 0 otherwise
int timer_expired(double seconds);



#endif // #ifndef __INCLUDE_TIMER_H__