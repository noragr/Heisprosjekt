#include "timer.h"
#include "elev.h"
#include <time.h>


void start_timer(){
	time(&start_value);  //time() returns seconds since pre-defined value
}

int timer_expired(double seconds){
	time(&end_value);
	if (difftime(end_value, start_value) >= seconds){ //double difftime(time_t time1, time_t time2) returns diff in sec between time1 and time2 (time1-time2)
		return 1; //timer expired
	}
	return 0;
}