# Readme 
This is the 4th assignment of *Data Processing On Modern Hardware*.
Please have a look at the assignment sheet before you start working!

## How to vectorize?
You're free to choose how you want to achieve vectorization in this task. We presented multiple options in the exercise session, but you are not limited. In general, you can use:
- Intel/ARM Intrinsics
- GCC Vector Extensions
- C inline assembly
- GCC immintrin.h
- Extern Assembly

## First Part
 - Implement the methods `toLowerScalar` and `toLowerVector` in `src/vectorization.c`.
 - Build the project with `make`, `make vector` or `make tester`.
 - You can try own configurations with the `vector` ELF created by `make`.
 - Test your implementation with the `tester` ELF created by `make tester`.

## Second part
- We provide a scalar implementation of Mandelbrot have a look!
- Vectorize the Mandelbrot computation

# Third part
- Write down your findings in a `report.pdf` and add it to the repo.
- This does not need to be an essay, so keep it short.
- You are free to visualize your findings!

## Technical Links
- [SSE/AVX Instructions](https://www.felixcloutier.com/x86/)
- [ARM SVE Intrinsics](https://developer.arm.com/documentation/100891/0612/coding-considerations/using-sve-intrinsics-directly-in-your-c-code)
- [ARM Neon](https://developer.arm.com/documentation/den0018/a/NEON-Intrinsics/Using-NEON-intrinsics)
- [GCC Vector Extensions](https://gcc.gnu.org/onlinedocs/gcc/Vector-Extensions.html)
- [Intel Intrinsics](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html)
- [C inline assembly](https://en.cppreference.com/w/c/language/asm)
- [GCC inline assembly guide](https://gcc.gnu.org/onlinedocs/gcc/Using-Assembly-Language-with-C.html)

## Visualization
- [Python: graphs and plots](https://www.geeksforgeeks.org/graph-plotting-in-python-set-1/)
- [Gnuplot](http://www.gnuplot.info/)