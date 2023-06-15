#ifndef VECTORIZATION_H
#define VECTORIZATION_H

#include <stdint.h>

// Returns the current time using CLOCK_MONOTONIC_RAW from Linux
double getCurTime();

// Functions
char *toLowerScalar(char *carray, uint32_t size);
char *toLowerScalar_nobranch(char *carray, uint32_t size);
char *toLowerVector(char *carray, uint32_t size);
void mandelbrotScalar(float xmin, float xmax, float ymin, float ymax, uint32_t maxiter, uint32_t width, uint32_t height, uint8_t *pixelarray);
void mandelbrotVector(float xmin, float xmax, float ymin, float ymax, uint32_t maxiter, uint32_t width, uint32_t height, uint8_t *pixelarray);
// Helpers
char *generateChars(uint32_t size);
uint64_t checksum(char *carray, uint32_t size);
void savePPM(uint8_t* pixelarray, uint32_t size, uint32_t width, char* filename);
#endif