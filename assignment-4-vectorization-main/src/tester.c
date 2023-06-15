#include <stdio.h>
#include <stdlib.h>
#include "CuTest.h"
#include "vectorization.h"

void tL1(CuTest *tc) {
    #define SIZE 100
    #define RESULT 92
    char *ca=generateChars(SIZE);
    uint64_t cs=checksum(toLowerScalar(ca,SIZE),SIZE);
    CuAssertTrue(tc, RESULT==cs);
    free(ca);
    #undef SIZE
    #undef RESULT
}

void tL2(CuTest *tc) {
    #define SIZE 1000
    #define RESULT 91
    char *ca=generateChars(SIZE);
    uint64_t cs=checksum(toLowerScalar(ca,SIZE),SIZE);
    CuAssertTrue(tc, RESULT==cs);
    free(ca);
    #undef SIZE
    #undef RESULT
}

void tL3(CuTest *tc) {
    #define SIZE 10000
    #define RESULT 92
    char *ca=generateChars(SIZE);
    uint64_t cs=checksum(toLowerScalar(ca,SIZE),SIZE);
    CuAssertTrue(tc, RESULT==cs);
    free(ca);
    #undef SIZE
    #undef RESULT
}

void tL4(CuTest *tc) {
    #define SIZE 500000
    #define RESULT 92
    char *ca=generateChars(SIZE);
    uint64_t cs=checksum(toLowerScalar(ca,SIZE),SIZE);
    CuAssertTrue(tc, RESULT==cs);
    free(ca);
    #undef SIZE
    #undef RESULT
}

void tL5(CuTest *tc) {
    #define SIZE 100
    #define RESULT 92
    char *ca=generateChars(SIZE);
    uint64_t cs=checksum(toLowerVector(ca,SIZE),SIZE);
    CuAssertTrue(tc, RESULT==cs);
    free(ca);
    #undef SIZE
    #undef RESULT
}

void tL6(CuTest *tc) {
    #define SIZE 1000
    #define RESULT 91
    char *ca=generateChars(SIZE);
    uint64_t cs=checksum(toLowerVector(ca,SIZE),SIZE);
    CuAssertTrue(tc, RESULT==cs);
    free(ca);
    #undef SIZE
    #undef RESULT
}

void tL7(CuTest *tc) {
    #define SIZE 10000
    #define RESULT 92
    char *ca=generateChars(SIZE);
    uint64_t cs=checksum(toLowerVector(ca,SIZE),SIZE);
    CuAssertTrue(tc, RESULT==cs);
    free(ca);
    #undef SIZE
    #undef RESULT
}

void tL8(CuTest *tc) {
    #define SIZE 500000
    #define RESULT 92
    char *ca=generateChars(SIZE);
    uint64_t cs=checksum(toLowerVector(ca,SIZE),SIZE);
    CuAssertTrue(tc, RESULT==cs);
    free(ca);
    #undef SIZE
    #undef RESULT
}

void tL9(CuTest *tc) {
    #define SIZE 100
    #define RESULT 92
    char *ca=generateChars(SIZE);
    uint64_t cs=checksum(toLowerScalar_nobranch(ca,SIZE),SIZE);
    CuAssertTrue(tc, RESULT==cs);
    free(ca);
    #undef SIZE
    #undef RESULT
}

void tL10(CuTest *tc) {
    #define SIZE 1000
    #define RESULT 91
    char *ca=generateChars(SIZE);
    uint64_t cs=checksum(toLowerScalar_nobranch(ca,SIZE),SIZE);
    CuAssertTrue(tc, RESULT==cs);
    free(ca);
    #undef SIZE
    #undef RESULT
}

void tL11(CuTest *tc) {
    #define SIZE 10000
    #define RESULT 92
    char *ca=generateChars(SIZE);
    uint64_t cs=checksum(toLowerScalar_nobranch(ca,SIZE),SIZE);
    CuAssertTrue(tc, RESULT==cs);
    free(ca);
    #undef SIZE
    #undef RESULT
}

void tL12(CuTest *tc) {
    #define SIZE 500000
    #define RESULT 92
    char *ca=generateChars(SIZE);
    uint64_t cs=checksum(toLowerScalar_nobranch(ca,SIZE),SIZE);
    CuAssertTrue(tc, RESULT==cs);
    free(ca);
    #undef SIZE
    #undef RESULT
}

CuSuite *toLowerSuite() {
    CuSuite *cs=CuSuiteNew();
    SUITE_ADD_TEST(cs, tL1);
    SUITE_ADD_TEST(cs, tL2);
    SUITE_ADD_TEST(cs, tL3);
    SUITE_ADD_TEST(cs, tL4);
    SUITE_ADD_TEST(cs, tL5);
    SUITE_ADD_TEST(cs, tL6);
    SUITE_ADD_TEST(cs, tL7);
    SUITE_ADD_TEST(cs, tL8);
    SUITE_ADD_TEST(cs, tL9);
    SUITE_ADD_TEST(cs, tL10);
    SUITE_ADD_TEST(cs, tL11);
    SUITE_ADD_TEST(cs, tL12);
    return cs;
}

int main()
{
    CuString *output = CuStringNew();
    CuSuite *suite = CuSuiteNew();

    CuSuiteAddSuite(suite, toLowerSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    if(suite->failCount != 0) {
        return 1;
    }
    return 0;
}