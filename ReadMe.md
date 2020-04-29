# Dot product benchmark

*Author: Tobit Flatscher (December 2019)

## Overview
These are several **cache-optimised implementations of the simple dot product** of two vectors. The dot product is calculated in parallel using **multi-threading with OpenMP** and **vectorisation** either with OpenMP or manually with **AVX2 or AVX512 intrinsics**.
Additionally the code makes use of templating, cache-aligned arrays (with Boost for vectors if available) and compile-time constexpr functions.
The different implementations are benchmarked against each other to check how well the compilers deal with auto-vectorisation for such a simple case. Ideally the compiler should perform similarly to the manual intrinsics. While this seems to be the case with the Intel compiler, GCC struggles to achieve the same performance.
Interestingly vectors do not seem to be fully cache-line aligned by both compilers even with optimisation flags such as `-O3`.

### The files

- `/bin` Folder for compiled binaries
- `/obj` Folder for object files
- `/src` Folder for source and header files
- `Makefile` The Make-file (default GCC) and settings for the ICC and GCC compiler
- `vec_vs_arr.cbp` CodeBlocks project files
- `bin/main.GCC` The executable compiled with GCC
- `bin/main.ICC` The executable compiled with ICC
- `src/align.hpp` Defines the cache-line-alignment relevant C++ macros (call to a Boost-vector if available)
- `src/avx2_omp.hpp` Implementation of dot-product by means of manual AVX2 intrinsics and multi-threading with OpenMP
- `src/avx512_omp.hpp` Implementation of dot-product by means of manual AVX512 intrinsics and multi-threading with OpenMP
- `src/avx_omp.hpp` Determine which version of AVX is available
- `src/benchmark.hpp` Generic functions for benchmarking
- `src/constexpr_func.hpp` The implementation of a square root with the recursive Newton-Raphson method that can be evaluated to constant expression at compile time
- `src/disclaimer.hpp` Prints out a disclaimer and tries to identify operating, compiler and features at compile time
- `src/init.hpp` Initialises vectors and arrays with random numbers
- `src/main.cpp` The main-file of this program
- `src/omp_simd.hpp` Implementation of dot-product by means of auto-vectorisation and multi-threading with OpenMP
- `src/span.hpp` [std::span](https://en.cppreference.com/w/cpp/container/span)-like container by [Tristan Brindle](https://github.com/tcbrindle/span) that will be introduced in C++20
- `src/timer.hpp` A simple wrapper for the chrono-library timer


## Launch it
Copy this folder or directly **clone this repository** by typing
```
$ git clone https://github.com/2b-t/OMP-AVX-intrinsics-dotprod.git 
```
Make sure if the Intel compiler collection is installed on your system by opening a console and typing
```
$ icc -v
```
If it is not available on your system [install it](https://software.intel.com/en-us/compilers) (it is free of charge for students) and change the corresponding lines of the Make-file `Makefile`
```
1  # Makefile
2  # Tobit Flatscher - github.com/2b-t (2020)
3
4  # Compiler settings (alternatively: export COMPILER=)
5  COMPILER = ICC
6
```
else you can leave it on `GCC` and use the Gnu Compiler Collection only.
Finally compile it by typing
```
$ make clean
$ make run
```
