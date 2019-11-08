#ifndef OMP_SIMD_H_INCLUDED
#define OMP_SIMD_H_INCLUDED

/**
 * \file     omp_simd.hpp
 * \brief    dot product calculated with OpenMP by means of SIMD and parallel for
 * \mainpage Dot product from two vectors that are either stored in vectors or C
 *           arrays. The code makes use of OpenMP's automatic vectorisation (SIMD)
 *           and parallel for.
 * \warning  The arrays should be cache aligned for optimal performance!
*/


/// include OpenMP only if available on machine
#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include <assert.h>
#include <vector>
#include "align.hpp"


/**\fn        omp_simd_vec
 * \brief     Calculate dot product of two vectors \p x and \p y, container:
 *            either C++ std vector or boost aligned C++ std vector.
 *
 * \param[in] x: a (un)aligned C++ or boosted aligned std vector
 * \param[in] y: a (un)aligned C++ or boosted aligned std vector
 * \return    Dot product of the two vectors
*/
template <typename T>
inline T omp_simd_vec(VEC(T) const &x, VEC(T) const &y)
{
    // check vector dimension
    assert(x.size() == y.size());

    T res = static_cast<T>(0.0);

    #pragma omp parallel for simd shared(x, y) reduction(+: res)
    for (size_t i = 0; i < x.size(); ++i)
    {
        res += x[i]*y[i];
    }

    return res;
}


/**\fn        omp_simd_arr
 * \brief     Calculate dot product of two vectors \p x and \p y, container: C array
 *
 * \param[in] x: a (un)aligned C array
 * \param[in] y: a (un)aligned C array
 * \return    Dot product of the two vectors
*/
template <typename T, size_t N>
inline T omp_simd_arr(T const (&x)[N], T const (&y)[N])
{
    T res = static_cast<T>(0.0);

    #pragma omp parallel for simd shared(x, y) reduction(+: res)
    for (size_t i = 0; i < N; ++i)
    {
        res += x[i]*y[i];
    }

    return res;
}

#endif // OMP_SIMD_H_INCLUDED
