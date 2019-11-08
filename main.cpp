#include <vector>
#include <string.h>
#include <memory>
#include <iostream>
#include <iomanip>
#include "disclaimer.hpp"
#include "align.hpp"
#include "init.hpp"
#include "omp_simd.hpp"
#include "avx_omp.hpp"
#include "benchmark.hpp"


int main(int argc, char** argv)
{
    /// benchmark settings
    size_t constexpr length     = 1e5;    //unpadded length of vector
    size_t constexpr it         = 100000; //number of iterations for determining performance
    size_t constexpr no_threads = 1;      //number of OpenMP threads
    omp_set_num_threads(no_threads);

    /// allocate variables
    // calculate padding manually
    size_t constexpr padding = ((CACHE_LINE - sizeof(INTR)*length % CACHE_LINE) % CACHE_LINE) / sizeof(INTR) ;
    size_t constexpr padded = length + padding;

    // allocate vector
    VEC(double) const x_vec = init_vec(length);
    VEC(double) const y_vec = init_vec(length);

    // allocate aligned and padded array
    alignas(CACHE_LINE) INTR x_arr[padded];
    alignas(CACHE_LINE) INTR y_arr[padded];
    // copy values from vector to array
    vec_to_arr(x_vec, x_arr);
    vec_to_arr(y_vec, y_arr);


    /// print disclaimer
    if ((argc > 1) && (strcmp(argv[1], "--version") == 0))
    {
        print_disclaimer();
    }


    /// check for alignment
    std::cout << std::endl;
    std::cout << "DATA ALIGNMENT" << std::endl;
    std::cout << "Vector (" << x_vec.size() << " elements):" << std::endl;
    test_alignment(&x_vec[0], length);
    std::cout << "Array ("  << padded       << " elements):" << std::endl;
    test_alignment(&x_arr[0], length);


    /// run benchmarks
    std::cout << std::endl;
    std::cout << "STARTING BENCHMARKS with " << it << " iterations" << std::endl;
    std::cout << std::fixed << std::setprecision(3) << std::setfill(' ');

    // vector: omp parallel for and simd
    std::cout << " -Vector OMP SIMD:  ";
    benchmark_vec(x_vec, y_vec, omp_simd_vec, it);

    // array: omp parallel for and simd
    std::cout << " -Array OMP SIMD:   ";
    benchmark_arr(x_arr, y_arr, omp_simd_arr, it);

    // array: manual avx2 vectorisation and omp parallel for
    #ifdef __AVX2__
    std::cout << " -Array AVX2 OMP:   ";
    benchmark_arr(x_arr, y_arr, avx2_omp_arr, it);
    #endif

    // array: manual avx512 vectorisation and omp parallel for
    #ifdef __AVX512CD__
    std::cout << " -Array AVX512 OMP: ";
    benchmark_arr(x_arr, y_arr, avx512_omp_arr, it);
    #endif

	return EXIT_SUCCESS;
}
