#ifndef AVX2_OMP_H_INCLUDED
#define AVX2_OMP_H_INCLUDED

/**
 * \file     avx2_omp.hpp
 * \brief    dot product calculated with AVX2 intrinsics and OpenMP
 * \mainpage Dot product: manual AVX2 implementation by means of double
 *           intrinsics with OpenMP parallel for with propietary reduction
 *           operation and partial loop unrolling so a single core always
 *           works  on its entire cache line. The arrays are assumed to be
 *           cache aligned and to be padded to be a multiple of the cache
 *           line size.
 * \warning  The arrays must be cache aligned!
*/

#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include <assert.h>
#include <vector>
#include "align.hpp"


#ifdef __AVX2__

// size of the intrinsic (AVX2: 256/8=32) and corresponding number of values of type INTR
#define AVX2_INTR_SIZE     sizeof(__m256d)
#define AVX2_REG_SIZE      sizeof(__m256d)/sizeof(INTR)

/// header for AVX2 intrinsics
#include <immintrin.h>

/// propietary OpenMP reduction operation for AVX2
#pragma omp declare reduction \
    (addpd: __m256d: omp_out += omp_in) \
    initializer(omp_priv = _mm256_setzero_pd())


/**\fn        _mm256_reduce_add_pd
 * \brief     Horizontal add function of all four numbers in a 256bit AVX2 double intrinsic
 *
 * \param[in] _a: a 256bit AVX2 intrinsic with 4 double numbers
 * \return    The horizontal added intrinsic as a double number
*/
static inline double _mm256_reduce_add_pd(__m256d _a)
{
    __m256d _sum = _mm256_hadd_pd(_a, _a);
    return ((double*)&_sum)[0] + ((double*)&_sum)[2];
}


/**\fn        avx2_omp_arr
 * \brief     Calculate dot product of two vectors \p x and \p y using 256bit
 *            AVX2 double intrinsics (4 double numbers, half a cache line),
 *            container: C array
 *
 * \param[in] x: a (un)aligned C array
 * \param[in] y: a (un)aligned C array
 * \return    Dot product of the two vectors
*/
template <size_t N>
inline double avx2_omp_arr(double const (&x)[N], double const (&y)[N])
{
    // divide cache line into two variables handled by the same processor
    __m256d _res1 = _mm256_setzero_pd();
    __m256d _res2 = _mm256_setzero_pd();

    #pragma omp parallel for shared(x, y) reduction(addpd: _res1) reduction(addpd: _res2)
    for (size_t i = 0; i < N-AVX2_REG_SIZE; i += 2*AVX2_REG_SIZE)
    {
        // use partial loop unrolling in order to cover entire cache line
        _res1 = _mm256_fmadd_pd(_mm256_load_pd(&x[i]),               _mm256_load_pd(&y[i]),               _res1);
        _res2 = _mm256_fmadd_pd(_mm256_load_pd(&x[i+AVX2_REG_SIZE]), _mm256_load_pd(&y[i+AVX2_REG_SIZE]), _res2);
    }

    // reduce all intrinsics to single double
    __m256d _res = _mm256_add_pd(_res1, _res2);
    return _mm256_reduce_add_pd(_res);
}

#endif //__AVX2__

#endif // AVX2_OMP_H_INCLUDED
