#include "timer.h"
#include "elev.h"
#include <time.h>
#include <stdio.h>


time_t start_time(){
	return time(NULL);  //time() returns seconds since pre-defined value
}


int timer_expired(time_t start_time){
	time_t end_time = time(NULL);
	if (end_time - start_time >= 3){ //double difftime(time_t time1, time_t time2) returns diff in sec between time1 and time2 (time1-time2)
		return 1; //timer expired
	}
	return 0;
}

