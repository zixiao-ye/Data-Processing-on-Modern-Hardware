#include "vectorization.h"
#include "prng.h"
#include <stdio.h>
#include <math.h>
#include <immintrin.h>

// Takes an array of chars and converts (scalar) all uppercase chars to lowercase ones
char *toLowerScalar(char *carray, uint32_t size){
    double start=getCurTime();
    //using mask to do the conversion
    uint32_t mask = 0b00100000;
    for (size_t i = 0; i < size; i++)
    {
        if (carray[i]>='A' && carray[i]<='Z')
        {
            carray[i] |= mask;
        }
    }

    double end = getCurTime();
    double time = end - start;
    printf("Scalar version toLower took: %.8lfs\n", time);
    
    return carray;
} 

char *toLowerScalar_nobranch(char *carray, uint32_t size){
    double start=getCurTime();
    //using mask to do the conversion
    uint32_t mask = 0b00100000;
    char masks_A = 'A';
    char masks_Z = 'Z';
    for (size_t i = 0; i < size; i++)
    {
        int8_t left_A = carray[i]>=masks_A;
        int8_t right_Z = carray[i]<=masks_Z;
        int8_t range = left_A&right_Z;
        //make the range to -1 (all bits are 1 for range)
        range = -range;
        carray[i]|=(range&mask);
    }

    double end = getCurTime();
    double time = end - start;
    printf("Scalar_nobranch version toLower took: %.8lfs\n", time);
    
    return carray;
}

// Takes an array of chars and converts (vector) all uppercase chars to lowercase ones
typedef char v8ch __attribute__((vector_size(8)));
char *toLowerVector(char *carray, uint32_t size){
    double start=getCurTime();
    uint32_t i = 0;
    //using mask to do the conversion
    uint32_t mask = 0b00100000;

    if (size>8)
    {
        v8ch masks_A = {'A','A','A','A','A','A','A','A'};
        v8ch masks_Z = {'Z','Z','Z','Z','Z','Z','Z','Z'};
        v8ch masks = {0b00100000,0b00100000,0b00100000,0b00100000,0b00100000,0b00100000,0b00100000,0b00100000};
        for (size_t i = 0; i < size; i+=8)
        {
            v8ch data = *(v8ch*)&carray[i];
            v8ch left_A = data>=masks_A;
            v8ch right_Z = data<=masks_Z;
            v8ch range = left_A&right_Z;
            data|=(range&masks);
            *(v8ch*)&carray[i]=data;
        }
        i-=8;
    }

    // deal with the remaining characters in the array when there are less than 8 characters
    while (i < size)
    {
        if (carray[i]>='A' && carray[i]<='Z')
        {
            carray[i] |= mask;
        }
    }

    double end = getCurTime();
    double time = end - start;
    printf("Vector version toLower took: %.8lfs\n", time);

    return carray;
}


// Computes Mandelbrot fractal (scalar)
void mandelbrotScalar(float xmin, float xmax, float ymin, float ymax, uint32_t maxiter, uint32_t width, uint32_t height, uint8_t *pixelarray) {
    double start=getCurTime();
    // Calculate size of pixel
    float dx=(xmax-xmin)/width;
    float dy=(ymax-ymin)/height;
    // Position in complex plane  
    float xp,yp;
    // Position of current point
    float u, v;
    // Position in pixel array
    uint32_t pos=0;
    for(int i=0; i<height; i++) {
        yp=ymax-(i*dy);
        for(int j=0; j<width; j++) {
            // Reset point data  
            float u=0.0;
            float v=0.0;
            float u2=0.0;
            float v2=0.0;
            xp=xmin+(j*dx);
            // Number of iterations
            uint32_t k;
            // Start iterations
                for(k=1; (k<maxiter)&&((u2+v2)<4.0); k++) {
                        v=(2*u*v)+yp;
                        u=u2-v2+xp;
                        u2=u*u;
                        v2=v*v;
                };
            // Set the colors of the pixels
            if(k>=maxiter) {
                // Black pixel
                pixelarray[pos]=0x0;
                pixelarray[pos+1]=0x0;
                pixelarray[pos+2]=0x0;
            }
            else {
                // Color pixel
                pixelarray[pos]=(uint8_t)(k&255);
                pixelarray[pos+1]=(uint8_t)((k>>8)&255);
                pixelarray[pos+2]=(uint8_t)((k>>16)&255);
            };
            pos+=3;
        }
  }
  double end=getCurTime();
  double time=end-start;
  printf("Scalar version Mandelbrot took: %.8lfs    Pixel Time: %.8lfs\n",time, time/((double)(height*width)));
}

/* typedef float v8sf __attribute__((vector_size(32)));
// Computes Mandelbrot fractal (vector)
void mandelbrotVector(float xmin, float xmax, float ymin, float ymax, uint32_t maxiter, uint32_t width, uint32_t height, uint8_t *pixelarray) {
    double start = getCurTime();

    // Calculate size of pixel
    float dx = (xmax - xmin) / width;
    float dy = (ymax - ymin) / height;

    // Position in pixel array
    uint32_t pos = 0;

    for (int i = 0; i < height; i++) {
        float yp = ymax - (i * dy);

        for (int j = 0; j < width; j += 8) {
            // Reset point data
            v8sf u = {0.0f};
            v8sf v = {0.0f};
            v8sf u2 = {0.0f};
            v8sf v2 = {0.0f};

            v8sf xp = {xmin + (j * dx), xmin + ((j + 1) * dx), xmin + ((j + 2) * dx), xmin + ((j + 3) * dx), xmin + ((j + 4) * dx), xmin + ((j + 5) * dx), xmin + ((j + 6) * dx), xmin + ((j + 7) * dx)};

            // Number of iterations
            uint32_t iter[8] = {0};

            // Start iterations
            for (int k = 1; k < maxiter; k++) {
                v = 2 * v * u + yp;
                u = u2 - v2 + xp;
                u2 = u * u;
                v2 = v * v;
                for (int m = 0; m < 8; m++) {
                    // Check if (u^2 + v^2) >= 4.0
                    if (u2[m] + v2[m] >= 4.0f) {
                        iter[m] = k;
                    }
                }

                // Check if all iterations have finished
                int all_done = 0;
                for (int m = 0; m < 8; m++) {
                    if (iter[m] !=0) {
                        all_done ++;
                        //break;
                    }
                }

                if (all_done==8) {
                    break;
                }
            }

            // Set the colors of the pixels
            for (int m = 0; m < 8; m++) {
                if (iter[m] >= maxiter) {
                    // Black pixel
                    pixelarray[pos] = 0x0;
                    pixelarray[pos + 1] = 0x0;
                    pixelarray[pos + 2] = 0x0;
                } else {
                    // Color pixel
                    pixelarray[pos] = (uint8_t)(iter[m] & 255);
                    pixelarray[pos + 1] = (uint8_t)((iter[m] >> 8) & 255);
                    pixelarray[pos + 2] = (uint8_t)((iter[m] >> 16) & 255);
                }
                pos += 3;
            }
        }
    }

    double end = getCurTime();
    double time = end - start;
    printf("Vector version Mandelbrot took: %.8lfs    Pixel Time: %.8lfs\n", time, time / ((double)(height * width)));
}  */


typedef float v4sf __attribute__((vector_size(16)));

void mandelbrotVector(float xmin, float xmax, float ymin, float ymax, uint32_t maxiter, uint32_t width, uint32_t height, uint8_t *pixelarray) {
    double start = getCurTime();

    // Calculate size of pixel
    float dx = (xmax - xmin) / width;
    float dy = (ymax - ymin) / height;

    // Position in pixel array
    uint32_t pos = 0;

    for (int i = 0; i < height; i++) {
        float yp = ymax - (i * dy);

        for (int j = 0; j < width; j += 4) {
            // Reset point data
            v4sf u = {0.0f};
            v4sf v = {0.0f};
            v4sf u2 = {0.0f};
            v4sf v2 = {0.0f};

            v4sf xp = {
                xmin + (j * dx), xmin + ((j + 1) * dx), xmin + ((j + 2) * dx), xmin + ((j + 3) * dx)
            };

            // Number of iterations
            uint32_t iter[4] = {0};

            // Start iterations
            for (int k = 1; k < maxiter; k++) {
                v = 2 * v * u + yp;
                u = u2 - v2 + xp;
                u2 = u * u;
                v2 = v * v;
                for (int m = 0; m < 4; m++) {
                    // Check if (u^2 + v^2) >= 4.0
                    if (u2[m] + v2[m] >= 4.0f) {
                        iter[m] = k;
                    }
                }

                // Check if all iterations have finished
                int all_done = 0;
                for (int m = 0; m < 4; m++) {
                    if (iter[m] != 0) {
                        all_done ++;
                    }
                }

                if (all_done == 4) {
                    break;
                }
            }

            // Set the colors of the pixels
            for (int m = 0; m < 4; m++) {
                if (iter[m] >= maxiter) {
                    // Black pixel
                    pixelarray[pos] = 0x0;
                    pixelarray[pos + 1] = 0x0;
                    pixelarray[pos + 2] = 0x0;
                } else {
                    // Color pixel
                    pixelarray[pos] = (uint8_t)(iter[m] & 255);
                    pixelarray[pos + 1] = (uint8_t)((iter[m] >> 8) & 255);
                    pixelarray[pos + 2] = (uint8_t)((iter[m] >> 16) & 255);
                }
                pos += 3;
            }
        }
    }

    double end = getCurTime();
    double time = end - start;
    printf("Vector version Mandelbrot took: %.8lfs    Pixel Time: %.8lfs\n", time, time / ((double)(height * width)));
}

