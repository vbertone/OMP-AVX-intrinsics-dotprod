#ifndef AVX512_OMP_H_INCLUDED
#define AVX512_OMP_H_INCLUDED

/**
 * \file     avx512_omp.hpp
 * \brief    dot product calculated with AVX512 intrinsics and OpenMP
 * \mainpage Dot product: manual AVX512 implementation by means of double
 *           intrinsics with OpenMP parallel for with propietary reduction
 *           operation. The arrays are assumed to be cache aligned and to
 *           be padded to be a multiple of the cache line size.
 * \warning  The arrays must be cache aligned!
*/


#if __has_include(<span>)
    #include <span>
#else
    #include "span.hpp"
    namespace std
    {
        using tcb::span;
    }
#endif

#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include <assert.h>
#include <vector>
#include "align.hpp"


#ifdef __AVX512CD__

/// header for AVX512 intrinsics
#if __has_include (<zmmintrin.h>)
    #include <zmmintrin.h>
#else
    #include <immintrin.h>
#endif

// size of the intrinsic (AVX512: 512/8=64) and corresponding number of values of type INTR
#define AVX512_INTR_SIZE     sizeof(__m512d)
#define AVX512_REG_SIZE      sizeof(__m512d)/sizeof(INTR)


/// propietary OpenMP reduction operation for AVX512
#pragma omp declare reduction \
    (addpd: __m512d: omp_out += omp_in) \
    initializer(omp_priv = _mm512_setzero_pd())


#ifndef __INTEL_COMPILER
/**\fn        _mm512_reduce_add_pd
 * \brief     Horizontal add function of all four numbers in a 512bit AVX2 double intrinsic
 *
 * \param[in] _a   a 512bit AVX512 intrinsic with 8 double numbers
 * \return    The horizontal added intrinsic as a double number
*/
static inline double _mm512_reduce_add_pd(__m512d _a)
{
    __m256d _b = _mm256_add_pd(_mm512_castpd512_pd256(_a), _mm512_extractf64x4_pd(_a, 1));
    __m128d _c = _mm_add_pd(_mm256_castpd256_pd128(_b), _mm256_extractf128_pd(_b, 1));
    double const *f = (double*)(&_c);
    return _mm_cvtsd_f64(_c) + f[1];
}
#endif


/**\fn        avx512_omp_span
 * \brief     Calculate dot product of two vectors \p x and \p y using 512bit
 *            AVX512 double intrinsics (8 double numbers, entire cache line),
 *            container: C++ span
 *
 * \param[in] x   an aligned C++ span
 * \param[in] y   an aligned C++ span
 * \return    Dot product of the two vectors
*/
inline double avx512_omp_span(std::span<double> const &x, std::span<double> const &y)
{
    assert(x.size() == y.size());
    size_t const N = x.size();

    __m512d _res = _mm512_setzero_pd();

    #pragma omp parallel for shared(x, y) reduction(addpd: _res)
    for (size_t i = 0; i < N; i += AVX512_REG_SIZE)
    {
        _res = _mm512_fmadd_pd(_mm512_load_pd(&x[i]), _mm512_load_pd(&y[i]), _res);
    }

    // reduce intrinsic to single double
    return _mm512_reduce_add_pd(_res);
}

#endif // __AVX512CD__

#endif // AVX512_OMP_H_INCLUDED
