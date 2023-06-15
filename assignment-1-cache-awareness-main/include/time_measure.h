#ifndef TIME_MEASURE_H_INCLUDED
#define TIME_MEASURE_H_INCLUDED

#include <time.h>

// Returns the current time using CLOCK_MONOTONIC_RAW from Linux
double getCurTime() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t);
    return t.tv_sec + t.tv_nsec*1e-9;
}

#endif