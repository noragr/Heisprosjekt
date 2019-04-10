#include "timer.h"
#include "elev.h"
#include <time.h>
#include <stdio.h>

/*
static time_t start_value;
static time_t end_value;*/


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







/*
clock_t start_timer(){
	return clock(); //returns nr secs since program launched when divided by CLOCKS_PER_SEC
}

int timer_expired(clock_t start_value){
	clock_t difference = clock() *1000- start_value;
	double seconds = difference  / CLOCKS_PER_SEC;
	if (seconds >= 3){ 
		printf("teeeest\n");
		return 1; //timer expired
	}
	return 0;
}*/