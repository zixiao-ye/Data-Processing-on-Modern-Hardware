#ifndef ARRPER_H_INCLUDED
#define ARRPER_H_INCLUDED

#include <stdint.h>

void seedRand();
uint64_t* generateArray(uint32_t asize);
uint32_t* generateIndexArray(uint32_t asize);
uint32_t* generateIndexArrayStrided(uint32_t asize, uint32_t stride);
uint64_t** generate_blocks(uint32_t block_size, uint32_t block_num);
void one_cyclic_permutation(uint32_t* array, uint32_t asize);

#endif