#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "vectorization.h"

int main(int argc, char **argv) {
    // Argument Handling
    switch(argc){
        case 2: goto toLowerArguments; break;
        case 8: goto manelbrotArguments; break;
        default: fprintf(stderr, "For toLowerCase use:\n\t./vector [NUMBER OF CHARACTERS]\nFor Mandelbrot use:\n\t./vector [XMIN] [XMAX] [YMIN] [YMAX] [MAX ITERATIONS] [IMAGE WIDTH IN PX] [NAME.ppm]\n\t./vector 0.27085 0.27100 0.004640 0.004810 1024 2048 DPMH.ppm\n"); return 0;
    }
    toLowerArguments:
        uint32_t nc;
        if(1!=sscanf(argv[1], "%u", &nc)){
            fprintf(stderr, "Define a valid number of characters\n");
            exit(1);
        }
        char *ca=generateChars(nc);
        uint64_t cca=checksum(ca,nc);
        double ss=getCurTime();
        uint64_t scca=checksum(toLowerScalar(ca,nc),nc);
        double se=getCurTime();
        free(ca);
        ca=generateChars(nc);
        double vs=getCurTime();
        uint64_t vcca=checksum(toLowerVector(ca,nc),nc);
        double ve=getCurTime();
        printf("Checksum Init: %lu\tChecksum Scalar: %lu in %.8lfs\t Checksum Vector: %lu in %.8lfs\n",cca,scca,(se-ss),vcca,(ve-vs));
        return 0;
    manelbrotArguments:
        float xmin,xmax,ymin,ymax;
        uint32_t maxiter,width;
        int err=sscanf(argv[1], "%f",&xmin);
        err &=sscanf(argv[2],"%f",&xmax);
        err &=sscanf(argv[3],"%f",&ymin);
        err &=sscanf(argv[4],"%f",&ymax);
        err &=sscanf(argv[5],"%u",&maxiter);
        err &=sscanf(argv[6],"%u",&width);
        if(err!=1){
            fprintf(stderr, "Define valid arguments for Mandelbrot\n");
            exit(1);
        }
        uint32_t height=(width*(ymax-ymin))/(xmax-xmin);
        uint8_t *pixelarray=(uint8_t*)malloc(width*height*3*sizeof(uint8_t));
        mandelbrotScalar(xmin,xmax,ymin,ymax,maxiter,width,height,pixelarray);
        mandelbrotVector(xmin,xmax,ymin,ymax,maxiter,width,height,pixelarray);
        savePPM(pixelarray,width*height*3*sizeof(uint8_t),width,argv[7]);
        return 0;
} 

