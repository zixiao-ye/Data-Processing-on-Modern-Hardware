#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "arrper.h"
#include "cache_access.h"

#define BLOCK_SIZE 4096

int main(int argc, char **argv) {
    uint32_t array_size, stride;
    if(argc != 3) {
        fprintf(stderr, "Invoke with [ArraySize] [STRIDE]\n");
        return 1;
    } else {
        int err = sscanf(argv[1], "%u", &array_size);
        err &= sscanf(argv[2], "%u", &stride);
        if(err!=1) {
            fprintf(stderr, "Invoke with [ArraySize] [STRIDE]\n");
            return 1;
        }
        if(stride < 1) {
            fprintf(stderr, "Stride must be greater or equal to one\n");
            return 1;
        }
    }

    seedRand();

    uint64_t *array = generateArray(array_size);
    uint32_t *oc = generateIndexArray(array_size);
    one_cyclic_permutation(oc, array_size);

    access_result seq_res, rseq_res, rand_res, chain_block_res, chain_prefetch_block_res;
    
    seq_res = sequential_access(array, array_size, stride);
    printf("Sequential access took %.6lfs\n", seq_res.time_taken);

    rseq_res = reverse_sequential_access(array, array_size, stride);
    printf("Reverse sequential access took %.6lfs\n", rseq_res.time_taken);

    rand_res = random_access(array, oc, array_size);
    printf("Random access took %.6lfs\n", rand_res.time_taken);

    free(array);
    free(oc);
    uint64_t **block_array = generate_blocks(BLOCK_SIZE, array_size);

    chain_block_res = chain_block_access(block_array, array_size, BLOCK_SIZE);
    printf("Block access took %.6lfs\n", chain_block_res.time_taken);

    chain_prefetch_block_res =  chain_prefetched_block_access(block_array, array_size, BLOCK_SIZE);
    printf("Prefetched block access took %.6lfs\n", chain_prefetch_block_res.time_taken);

    free(block_array);
    return 0;
}