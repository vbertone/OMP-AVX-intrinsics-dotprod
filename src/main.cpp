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
#include "constexpr_func.hpp"


int main(int argc, char** argv)
{
    /// benchmark settings
    constexpr size_t no_threads = 1;  //number of OpenMP threads
    omp_set_num_threads(no_threads);

    constexpr size_t length = 1e5;    //unpadded length of vector
    constexpr size_t it     = 100000; //number of iterations for determining performance

    /// allocate variables
    // calculate padding manually
    constexpr size_t padding = PAD(length, INTR);
    constexpr size_t padded  = length + padding;

    // allocate vector
    VEC(double) const x_vec = init_vec(length);
    VEC(double) const y_vec = init_vec(length);

    // allocate aligned and padded array
    alignas(CACHE_LINE) std::array<INTR,padded> x_arr;
    alignas(CACHE_LINE) std::array<INTR,padded> y_arr;
    // copy values from vector to array
    vec_to_arr(x_vec, x_arr);
    vec_to_arr(y_vec, y_arr);


    /// print disclaimer
    if ( (argc > 1) && ((strcmp(argv[1], "--version") == 0) || (strcmp(argv[1], "--v") == 0)) )
    {
        print_disclaimer();
        exit(EXIT_SUCCESS);
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
    std::cout << " -C++ Vector OMP SIMD:   ";
    benchmark_fun(x_vec, y_vec, omp_simd_vec, it);

    // span: omp parallel for and simd
    std::cout << " -C++ Span   OMP SIMD:   ";
    benchmark_fun<std::span<INTR>>(x_arr, y_arr, omp_simd_span, it);

    // array: omp parallel for and simd
    std::cout << " -C++ Array  OMP SIMD:   ";
    benchmark_fun(x_arr, y_arr, omp_simd_arr, it);

    // array: manual avx2 vectorisation and omp parallel for
    #ifdef __AVX2__
        std::cout << " -C++ Array  AVX2 OMP:   ";
        benchmark_fun<std::span<INTR>>(x_arr, y_arr, avx2_omp_span, it);
    #endif

    // array: manual avx512 vectorisation and omp parallel for
    #ifdef __AVX512CD__
        std::cout << " -C++ Array  AVX512 OMP: ";
        benchmark_fun<std::span<INTR>>(x_arr, y_arr, avx512_omp_span, it);
    #endif

	return EXIT_SUCCESS;
}
