#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "prng.h"
#include "arrper.h"

// Seed the random number generator
void seedRand() {
    // Inits pseudo-random-generator with a FIXED seed. 
    // This guarantees that the generated sequence of random integers is the same everywhere. 
    // Do NOT do this if you need real pseudo random numbers!
    uint8_t seed[] = {1,3,5,7,11,13,17,19,23,29};
    prng_seed_bytes(seed, 10);
}

// Generates an array of uint64_t
uint64_t* generateArray(uint32_t asize) {
    uint64_t *ptr = (uint64_t*) malloc(asize*sizeof(uint64_t));
    if(!ptr) {
        fprintf(stderr, "Could not allocate memory for random numbers\n");
        exit(1);
    }
    for(uint32_t i = 0; i< asize; i++) {
        ptr[i] = (uint64_t) i;
    }
    return ptr;
}

// Generates an array filled with indexes
uint32_t* generateIndexArray(uint32_t asize) {
    uint32_t *ptr = (uint32_t*) malloc(asize*sizeof(uint32_t));
    if(!ptr) {
        fprintf(stderr, "Could not allocate memory for index array\n");
        exit(1);
    }
    for(uint32_t i = 0; i< asize; i++) {
        ptr[i] = i;
    }
    return ptr;
}

// Generates an array filled with strided indexes
uint32_t* generateIndexArrayStrided(uint32_t asize, uint32_t stride) {
    uint32_t *ptr = (uint32_t*) malloc(asize*sizeof(uint64_t));
    if(!ptr) {
        fprintf(stderr, "Could not allocate memory for index array\n");
        exit(1);
    }
    uint32_t cur_off = 0;
    for(uint32_t i = 0; cur_off < asize; i++) {
        ptr[i] = cur_off;
        cur_off += stride;
    }
    return ptr;
}

uint64_t** generate_blocks(uint32_t block_size, uint32_t block_num) {
    uint64_t **ptr = (uint64_t**) malloc(block_num*sizeof(uint64_t*));
    if(!ptr) {
        fprintf(stderr, "Could not allocate memory for array of blocks\n");
        exit(1);
    }
    for(uint32_t i = 0; i < block_num; i++) {
        ptr[i]=generateArray(block_size);
    }
    return ptr;
}