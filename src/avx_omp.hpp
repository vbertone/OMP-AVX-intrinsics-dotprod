#ifndef AVX_OMP_H_INCLUDED
#define AVX_OMP_H_INCLUDED

/**
 * \file     avx_omp.hpp
 * \mainpage Decide which version of AVX should be used.
*/


#include "avx2_omp.hpp"
#include "avx512_omp.hpp"


/// check which intrinsics vectorisation the compiler and computer support
#ifdef __AVX512CD__
    #define AVX_SUP
    #define avx_omp_span avx512_omp_span
#elif __AVX2__
    #define AVX_SUP
    #define avx_omp_span avx2_omp_span
#endif

#endif // AVX_OMP_H_INCLUDED
