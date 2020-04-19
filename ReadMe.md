# Dot product benchmark

*Author: Tobit Flatscher (December 2019)

## Overview
These are several **cache-optimised implementations of the simple dot product** of two vectors. The dot product is calculated in parallel using **multi-threading with OpenMP** and **vectorisation** either with OpenMP or manually with **AVX2 or AVX512 intrinsics**.
Additionally the code makes use of templating, cache-aligned arrays (with Boost for vectors if available) and compile-time constexpr functions.
The different implementations are benchmarked against each other to check how well the compilers deal with auto-vectorisation for such a simple case. Ideally the compiler should perform similarly to the manual intrinsics. While this seems to be the case with the Intel compiler, GCC struggles to achieve the same performance.
Interestingly vectors do not seem to be fully cache-line aligned by both compilers even with optimisation flags such as `-O3`.

### The files

- `GCC_default.mk` Make settings for GNU Compiler Collection GCC compiler
- `ICC_default.mk` Make settings for Intel Compiler Collection ICC (if available)
- `Makefile` The Make-file (default ICC)
- `align.hpp` Defines the cache-line-alignment relevant C++ macros (call to a Boost-vector if available)
- `avx2_omp.hpp` Implementation of dot-product by means of manual AVX2 intrinsics and multi-threading with OpenMP
- `avx512_omp.hpp` Implementation of dot-product by means of manual AVX512 intrinsics and multi-threading with OpenMP
- `avx_omp.hpp` Determine which version of AVX is available
- `benchmark.hpp` Generic functions for benchmarking
- `constexpr_func.hpp` The implementation of a square root with the recursive Newton-Raphson method that can be evaluated to constant expression at compile time
- `disclaimer.hpp` Prints out a disclaimer and tries to identify operating, compiler and features at compile time
- `init.hpp` Initialises vectors and arrays with random numbers
- `main.GCC_` The executable compiled with GCC
- `main.ICC_` The executable compiled with ICC
- `main.cpp` The main-file of this program
- `omp_simd.hpp` Implementation of dot-product by means of auto-vectorisation and multi-threading with OpenMP
- `span.hpp` [std::span](https://en.cppreference.com/w/cpp/container/span)-like container by [Tristan Brindle](https://github.com/tcbrindle/span) that will be introduced in C++20
- `timer.hpp` A simple wrapper for the chrono-library timer
- `vec_vs_arr.cbp`, `vec_vs_arr.depend` and `vec_vs_arr.layout` CodeBlocks project files


## Launch it
Copy this folder or directly **clone this repository** by typing
```
$ git clone https://github.com/2b-t/OMP-AVX-intrinsics-dotprod.git 
```
Make sure if the Intel compiler collection is installed on your system by opening a console and typing
```
$ icc -v
```
If it is not available on your system [install it](https://software.intel.com/en-us/compilers) (it is free of charge for students) or change the corresponding lines of the Make-file `Makefile`
```
3  #use GNU compiler collection
4  COMPILER = GCC_
5  #use Intel compiler
6  # COMPILER = ICC_
```
Finally compile it by typing
```
$ make clean
$ make run
```
