
#include <stdint.h>
#include "cache_access.h"
#include "arrper.h"
#include "time_measure.h"
#include "prng.h"

#include<stdio.h>


/*
 * Helpful functions: 
 * uint32_t prng_get_uint() returns a random number in [0, UINT32_MAX]
 * double getCurTime() returns the current time in nanoseconds
*/

/*
 * Shuffle the index array in array with size asize, so that it is a one cyclic permutation.
*/
void one_cyclic_permutation(uint32_t* array, uint32_t asize) {
    // TODO: YOUR CODE HERE
    uint32_t rand,temp;
    for (size_t i = asize-1; i > 0; i--)
    {
        rand = prng_get_uint()*i/UINT32_MAX;
        temp = array[rand];
        array[rand] = array[i];
        array[i] = temp;
    }
}

/*
 * Iterate over the numbers in array with a given stride. 
*/
access_result sequential_access(uint64_t *array, uint32_t asize, uint32_t stride) {
    // TODO: YOUR CODE HERE

    uint64_t result = 0;

    double t1 = getCurTime();
    for (size_t i = 0; i < asize; i+=stride)
    {
        result += array[i];  
    }
    double t2 = getCurTime();
    return (access_result) {t2-t1,result};
}

/*
 * Iterate over the numbers in array with a given stride, but start at the end. 
*/
access_result reverse_sequential_access(uint64_t *array, uint32_t asize, uint32_t stride) {
    // TODO: YOUR CODE HERE
    
    uint64_t result = 0;

    double t1 = getCurTime();
    for (size_t i = 0; i < asize; i+=stride)
    {
        result += array[asize-1-i];  
    }
    double t2 = getCurTime();
    return (access_result) {t2-t1,result};
}

/*
 * Sum up the numbers in array, but iterate over it using the offsets provided in offsets. 
*/
access_result random_access(uint64_t *array, uint32_t *offsets, uint32_t osize) {
    // TODO: YOUR CODE HERE
    
    uint64_t result = 0;
    double t1 = getCurTime();
    for (size_t i = 0; i < osize; i++)
    {
        result += array[offsets[i]];  
    }
    double t2 = getCurTime();
    return (access_result) {t2-t1,result};
}

/*
 * Array points to blocks containing block_size uint64_t.
 * Sum up the numbers over all blocks.
*/
access_result chain_block_access(uint64_t **array, uint32_t asize, uint32_t block_size) {
    // TODO: YOUR CODE HERE
    
    uint64_t result = 0, block_sum = 0;
    
    double t1 = getCurTime();
    for (size_t i = 0; i < asize; i++)
    {
        for (size_t j = 0; j < block_size; j++)
        {
            block_sum += array[i][j];
        }
        result += block_sum;
        block_sum = 0;
    } 
    double t2 = getCurTime();
    return (access_result) {t2-t1,result};
}

/*
 * Array points to blocks containing block_size uint64_t.
 * Sum up the numbers over all blocks.
 * Add prefetching to your previous solution.
*/
access_result chain_prefetched_block_access(uint64_t **array, uint32_t asize, uint32_t block_size) {
    // TODO: YOUR CODE HERE
    
    uint64_t result = 0, block_sum = 0;
    
    double t1 = getCurTime();
    for (size_t i = 0; i < asize; i++)
    {
        for (size_t j = 0; j < block_size; j++)
        {
            block_sum += array[i][j];
            __builtin_prefetch(&array[i][j+96], 1, 3);
        }
        result += block_sum;
        block_sum = 0;
    } 
    double t2 = getCurTime();
    return (access_result) {t2-t1,result};
}
