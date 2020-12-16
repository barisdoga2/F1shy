#include "Utils.h"

long long getCurrentMillis() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

void mssleep(long long ms){
    usleep(ms * 1000);
}