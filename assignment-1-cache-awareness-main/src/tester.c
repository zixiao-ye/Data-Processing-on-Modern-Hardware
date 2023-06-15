#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "arrper.h"
#include "cache_access.h"
#include "CuTest.h"

// Check if a permutation is one cyclic
int check_cyclic_permutation(uint32_t *array, uint32_t asize)
{
    if (asize <= 1)
        return 1;
    uint32_t start = array[0];
    uint32_t cur = start;
    for (uint32_t i = 0; i < (asize-1); i++)
    {
        cur = array[cur];
        if (cur == start) {
            return 0;
        }
    }
    return 1;
}

// ----------------------------------------------------------------
// TESTS FOR PERMUTATION
// ----------------------------------------------------------------

void permutation_1(CuTest *tc)
{
#define IA_SIZE 2
    uint32_t *ia = generateIndexArray(IA_SIZE);
    one_cyclic_permutation(ia, IA_SIZE);
    CuAssertTrue(tc, 1==check_cyclic_permutation(ia, IA_SIZE));
    free(ia);
#undef IA_SIZE
}

void permutation_2(CuTest *tc)
{
#define IA_SIZE 100
    uint32_t *ia = generateIndexArray(IA_SIZE);
    one_cyclic_permutation(ia, IA_SIZE);
    CuAssertTrue(tc, check_cyclic_permutation(ia, IA_SIZE));
    free(ia);
#undef IA_SIZE
}

void permutation_3(CuTest *tc)
{
#define IA_SIZE 10000
    uint32_t *ia = generateIndexArray(IA_SIZE);
    one_cyclic_permutation(ia, IA_SIZE);
    CuAssertTrue(tc, check_cyclic_permutation(ia, IA_SIZE));
    free(ia);
#undef IA_SIZE
}

void permutation_4(CuTest *tc)
{
#define IA_SIZE 1000000
    uint32_t *ia = generateIndexArray(IA_SIZE);
    one_cyclic_permutation(ia, IA_SIZE);
    CuAssertTrue(tc, check_cyclic_permutation(ia, IA_SIZE));
    free(ia);
#undef IA_SIZE
}

CuSuite *permutation_suite()
{
    CuSuite *cs = CuSuiteNew();
    SUITE_ADD_TEST(cs, permutation_1);
    SUITE_ADD_TEST(cs, permutation_2);
    SUITE_ADD_TEST(cs, permutation_3);
    SUITE_ADD_TEST(cs, permutation_4);
    return cs;
}

// ----------------------------------------------------------------
// TESTS FOR SEQUENTIAL ACCESS
// ----------------------------------------------------------------

void sequential_1(CuTest *tc)
{
#define A_SIZE 100
#define STRIDE 1
    uint64_t RESULT = 4950ULL;
    uint64_t *a = generateArray(A_SIZE);
    access_result ar = sequential_access(a, A_SIZE, STRIDE);
    CuAssertTrue(tc, (RESULT == ar.result));
    CuAssertTrue(tc, 0 < ar.time_taken);
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef STRIDE

}

void sequential_2(CuTest *tc)
{
#define A_SIZE 1024
#define STRIDE 1
    uint64_t RESULT = 523776ULL;
    uint64_t *a = generateArray(A_SIZE);
    access_result ar = sequential_access(a, A_SIZE, STRIDE);
    CuAssertTrue(tc, RESULT == ar.result);
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef STRIDE

}

void sequential_3(CuTest *tc)
{
#define A_SIZE 1024
#define STRIDE 9
    uint64_t RESULT = 57969ULL;
    uint64_t *a = generateArray(A_SIZE);
    access_result ar = sequential_access(a, A_SIZE, STRIDE);
    CuAssertTrue(tc, RESULT == ar.result);
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef STRIDE

}

void sequential_4(CuTest *tc)
{
#define A_SIZE 512000
#define STRIDE 1
    uint64_t RESULT = 131071744000ULL;
    uint64_t *a = generateArray(A_SIZE);
    access_result ar = sequential_access(a, A_SIZE, STRIDE);
    CuAssertTrue(tc, RESULT == ar.result);
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef STRIDE

}

void sequential_5(CuTest *tc)
{
#define A_SIZE 512000
#define STRIDE 131
    uint64_t RESULT = 1000602366ULL;
    uint64_t *a = generateArray(A_SIZE);
    access_result ar = sequential_access(a, A_SIZE, STRIDE);
    CuAssertTrue(tc, RESULT == ar.result);
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef STRIDE

}

void sequential_6(CuTest *tc)
{
#define A_SIZE 100
#define STRIDE 1
    uint64_t RESULT = 4950ULL;
    uint64_t *a = generateArray(A_SIZE);
    access_result ar = reverse_sequential_access(a, A_SIZE, STRIDE);
    CuAssertTrue(tc, RESULT == ar.result);
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef STRIDE

}

void sequential_7(CuTest *tc)
{
#define A_SIZE 1024
#define STRIDE 1
    uint64_t RESULT = 523776ULL;
    uint64_t *a = generateArray(A_SIZE);
    access_result ar = reverse_sequential_access(a, A_SIZE, STRIDE);
    CuAssertTrue(tc, RESULT == ar.result);
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef STRIDE

}

void sequential_8(CuTest *tc)
{
#define A_SIZE 1024
#define STRIDE 9
    uint64_t RESULT = 58647ULL;
    uint64_t *a = generateArray(A_SIZE);
    access_result ar = reverse_sequential_access(a, A_SIZE, STRIDE);
    CuAssertTrue(tc, ((RESULT == ar.result)  || (58653ULL == ar.result)));
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef STRIDE

}

void sequential_9(CuTest *tc)
{
#define A_SIZE 512000
#define STRIDE 1
    uint64_t RESULT = 131071744000ULL;
    uint64_t *a = generateArray(A_SIZE);
    access_result ar = reverse_sequential_access(a, A_SIZE, STRIDE);
    CuAssertTrue(tc, RESULT == ar.result);
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef STRIDE

}

void sequential_10(CuTest *tc)
{
#define A_SIZE 512000
#define STRIDE 131
    uint64_t RESULT = 1000801674ULL;
    uint64_t *a = generateArray(A_SIZE);
    access_result ar = reverse_sequential_access(a, A_SIZE, STRIDE);
    CuAssertTrue(tc, ((RESULT == ar.result) || (1000801725ULL  == ar.result)));
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef STRIDE

}

CuSuite *sequential_suite()
{
    CuSuite *cs = CuSuiteNew();
    SUITE_ADD_TEST(cs, sequential_1);
    SUITE_ADD_TEST(cs, sequential_2);
    SUITE_ADD_TEST(cs, sequential_3);
    SUITE_ADD_TEST(cs, sequential_4);
    SUITE_ADD_TEST(cs, sequential_5);
    SUITE_ADD_TEST(cs, sequential_6);
    SUITE_ADD_TEST(cs, sequential_7);
    SUITE_ADD_TEST(cs, sequential_8);
    SUITE_ADD_TEST(cs, sequential_9);
    SUITE_ADD_TEST(cs, sequential_10);
    return cs;
}

// ----------------------------------------------------------------
// TESTS FOR RANDOM ACCESS
// ----------------------------------------------------------------

void random_1(CuTest *tc)
{
#define A_SIZE 1000
#define STRIDE 1
uint64_t RESULT = 499500ULL;
    uint64_t *a = generateArray(A_SIZE);
    uint32_t *ia = generateIndexArray(A_SIZE);
    one_cyclic_permutation(ia, A_SIZE);
    access_result ar = random_access(a, ia, A_SIZE);
    CuAssertTrue(tc, RESULT == ar.result);
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
    free(ia);
#undef A_SIZE
#undef STRIDE

}

void random_2(CuTest *tc)
{
#define A_SIZE 10000
#define STRIDE 1
uint64_t RESULT = 49995000ULL;
    uint64_t *a = generateArray(A_SIZE);
    uint32_t *ia = generateIndexArray(A_SIZE);
    one_cyclic_permutation(ia, A_SIZE);
    access_result ar = random_access(a, ia, A_SIZE);
    CuAssertTrue(tc, RESULT == ar.result);
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
    free(ia);
#undef A_SIZE
#undef STRIDE

}

void random_3(CuTest *tc)
{
#define A_SIZE 100000
#define STRIDE 1
uint64_t RESULT = 4999950000ULL;
    uint64_t *a = generateArray(A_SIZE);
    uint32_t *ia = generateIndexArray(A_SIZE);
    one_cyclic_permutation(ia, A_SIZE);
    access_result ar = random_access(a, ia, A_SIZE);
    CuAssertTrue(tc, RESULT == ar.result);
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
    free(ia);
#undef A_SIZE
#undef STRIDE

}

void random_4(CuTest *tc)
{
#define A_SIZE 1000000
#define STRIDE 1
uint64_t RESULT = 499999500000ULL;
    uint64_t *a = generateArray(A_SIZE);
    uint32_t *ia = generateIndexArray(A_SIZE);
    one_cyclic_permutation(ia, A_SIZE);
    access_result ar = random_access(a, ia, A_SIZE);
    CuAssertTrue(tc, RESULT == ar.result);
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
    free(ia);
#undef A_SIZE
#undef STRIDE

}

CuSuite *random_suite()
{
    CuSuite *cs = CuSuiteNew();
    SUITE_ADD_TEST(cs, random_1);
    SUITE_ADD_TEST(cs, random_2);
    SUITE_ADD_TEST(cs, random_3);
    SUITE_ADD_TEST(cs, random_4);
    return cs;
}

// ----------------------------------------------------------------
// TESTS FOR PREFETCHED RANDOM ACCESS
// ----------------------------------------------------------------

void block_1(CuTest *tc)
{
#define A_SIZE 1000
#define BLOCK_SIZE 1000
uint64_t RESULT = 499500000ULL;
    uint64_t **a = generate_blocks(BLOCK_SIZE, A_SIZE);
    access_result ar = chain_block_access(a, A_SIZE, BLOCK_SIZE);
    CuAssertTrue(tc, RESULT == ar.result);
    for (uint32_t i = 0; i < A_SIZE; i++)
    {
        free(a[i]);
    }
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef BLOCK_SIZE

}

void block_2(CuTest *tc)
{
#define A_SIZE 10000
#define BLOCK_SIZE 1000
uint64_t RESULT = 4995000000ULL;
    uint64_t **a = generate_blocks(BLOCK_SIZE, A_SIZE);
    access_result ar = chain_block_access(a, A_SIZE, BLOCK_SIZE);
    CuAssertTrue(tc, RESULT == ar.result);
    for (uint32_t i = 0; i < A_SIZE; i++)
    {
        free(a[i]);
    }
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef BLOCK_SIZE

}

void block_3(CuTest *tc)
{
#define A_SIZE 100
#define BLOCK_SIZE 10000
uint64_t RESULT = 4999500000ULL;
    uint64_t **a = generate_blocks(BLOCK_SIZE, A_SIZE);
    access_result ar = chain_block_access(a, A_SIZE, BLOCK_SIZE);
    CuAssertTrue(tc, RESULT == ar.result);
    for (uint32_t i = 0; i < A_SIZE; i++)
    {
        free(a[i]);
    }
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef BLOCK_SIZE

}

void prefetch_1(CuTest *tc)
{
#define A_SIZE 1000
#define BLOCK_SIZE 1000
uint64_t RESULT = 499500000ULL;
    uint64_t **a = generate_blocks(BLOCK_SIZE, A_SIZE);
    access_result ar = chain_prefetched_block_access(a, A_SIZE, BLOCK_SIZE);
    CuAssertTrue(tc, RESULT == ar.result);
    for (uint32_t i = 0; i < A_SIZE; i++)
    {
        free(a[i]);
    }
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef BLOCK_SIZE

}

void prefetch_2(CuTest *tc)
{
#define A_SIZE 10000
#define BLOCK_SIZE 1000
uint64_t RESULT = 4995000000ULL;
    uint64_t **a = generate_blocks(BLOCK_SIZE, A_SIZE);
    access_result ar = chain_prefetched_block_access(a, A_SIZE, BLOCK_SIZE);
    CuAssertTrue(tc, RESULT == ar.result);
    for (uint32_t i = 0; i < A_SIZE; i++)
    {
        free(a[i]);
    }
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef BLOCK_SIZE

}

void prefetch_3(CuTest *tc)
{
#define A_SIZE 100
#define BLOCK_SIZE 10000
uint64_t RESULT = 4999500000ULL;
    uint64_t **a = generate_blocks(BLOCK_SIZE, A_SIZE);
    access_result ar = chain_prefetched_block_access(a, A_SIZE, BLOCK_SIZE);
    CuAssertTrue(tc, RESULT == ar.result);
    for (uint32_t i = 0; i < A_SIZE; i++)
    {
        free(a[i]);
    }
    CuAssertTrue(tc, 0 < ar.time_taken);
free(a);
#undef A_SIZE
#undef BLOCK_SIZE

}

CuSuite *prefetch_suite()
{
    CuSuite *cs = CuSuiteNew();
    SUITE_ADD_TEST(cs, block_1);
    SUITE_ADD_TEST(cs, block_2);
    SUITE_ADD_TEST(cs, block_3);
    SUITE_ADD_TEST(cs, prefetch_1);
    SUITE_ADD_TEST(cs, prefetch_2);
    SUITE_ADD_TEST(cs, prefetch_3);
    return cs;
}

int main()
{
    seedRand();
    CuString *output = CuStringNew();
    CuSuite *suite = CuSuiteNew();

    CuSuiteAddSuite(suite, permutation_suite());
    CuSuiteAddSuite(suite, sequential_suite());
    CuSuiteAddSuite(suite, random_suite());
    CuSuiteAddSuite(suite, prefetch_suite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    if(suite->failCount != 0) {
        return 1;
    }
    return 0;
}
