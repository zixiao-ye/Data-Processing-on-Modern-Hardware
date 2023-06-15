#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "vectorization.h"
#include "prng.h"

// Returns the current time using CLOCK_MONOTONIC_RAW from Linux
double getCurTime() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t);
    return t.tv_sec + t.tv_nsec*1e-9;
}

// Creates random characters
char *generateChars(uint32_t size) {
    uint8_t seed[6]={1,3,5,7,11,13};
    prng_seed_bytes((void*)seed,6);
    char *ca=(char*)malloc(size*sizeof(char));
    if(!ca) {
        fprintf(stderr, "Could not allocate char array...");
        exit(1);
    }
    for(uint32_t i=0; i<size; i++){
        ca[i]=48+prng_get_byte()/(256/(123-48+1)+1);
    }
    return ca;
}

// Computes a very _basic_ checksum
uint64_t checksum(char *carray, uint32_t size) {
    uint64_t csum=0;
    for(uint32_t i=0; i<size; i++){
        csum+=(uint64_t)carray[i];
    }
    return csum/size;
}

// Creates a PPM file and writes contens of pixelarray to it
void savePPM(uint8_t* pixelarray, uint32_t size, uint32_t width, char* filename) {
    FILE *fp = fopen(filename, "wb");
    if(!fp) {
        fprintf(stderr, "Could not create PPM file...");
        exit(1);
    }
    // Calculate height
    uint32_t height=(size/3)/width;
    // Create file header
    fprintf(fp, "P6\n# DPMH FOREVER\n%u %u\n255\n",width,height);
    fwrite(pixelarray,1,size,fp);
    fclose(fp);
}

