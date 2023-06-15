#ifndef CACHE_ACCESS_H_INCLUDED
#define CACHE_ACCESS_H_INCLUDED

#include <stdint.h>

/*
 * Struct encapsulating time taken and result
*/
typedef struct _access_result {
    double time_taken;
    uint64_t result;
} access_result;

// Function definitions
access_result sequential_access(uint64_t *array, uint32_t asize, uint32_t stride);
access_result reverse_sequential_access(uint64_t *array, uint32_t asize, uint32_t stride);
access_result random_access(uint64_t *array, uint32_t *offsets, uint32_t osize);
access_result chain_block_access(uint64_t **array, uint32_t asize, uint32_t block_size);
access_result chain_prefetched_block_access(uint64_t **array, uint32_t asize, uint32_t block_size);

#endif