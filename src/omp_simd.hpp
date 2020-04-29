#ifndef OMP_SIMD_H_INCLUDED
#define OMP_SIMD_H_INCLUDED

/**
 * \file     omp_simd.hpp
 * \brief    dot product calculated with OpenMP by means of SIMD and parallel for
 * \mainpage Dot product from two vectors that are either stored in any contiguous
 *           C++ container. The code makes use of OpenMP's automatic vectorisation
 *           (SIMD) and parallel for.
 * \warning  The arrays should be cache aligned for optimal performance!
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


/**\fn        omp_simd_vec
 * \brief     Calculate dot product of two vectors \p x and \p y, container:
 *            either C++ std vector or boost aligned C++ std vector.
 *
 * \param[in] x   a (un)aligned C++ or boosted aligned std vector
 * \param[in] y   a (un)aligned C++ or boosted aligned std vector
 * \return    Dot product of the two vectors
*/
template <typename T>
inline T omp_simd_vec(VEC(T) const &x, VEC(T) const &y)
{
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
 * \brief     Calculate dot product of two vectors \p x and \p y, container: C++ array
 *
 * \param[in] x   a (un)aligned C++ array
 * \param[in] y   a (un)aligned C++ array
 * \return    Dot product of the two vectors
*/
template <typename T = double, size_t N>
inline T omp_simd_arr(std::array<T,N> const &x, std::array<T,N> const &y)
{
    T res = static_cast<T>(0.0);

    #pragma omp parallel for simd shared(x, y) reduction(+: res)
    for (size_t i = 0; i < N; ++i)
    {
        res += x[i]*y[i];
    }

    return res;
}


/**\fn        omp_simd_span
 * \brief     Calculate dot product of two vectors \p x and \p y, container: C++ span
 *
 * \param[in] x   a (un)aligned C++ span
 * \param[in] y   a (un)aligned C++ span
 * \return    Dot product of the two vectors
*/
template <typename T = double>
inline T omp_simd_span(std::span<T> const &x, std::span<T> const &y)
{
    assert(x.size() == y.size());
    size_t const N = x.size();

    T res = static_cast<T>(0.0);

    #pragma omp parallel for simd shared(x, y) reduction(+: res)
    for (size_t i = 0; i < N; ++i)
    {
        res += x[i]*y[i];
    }

    return res;
}

#endif // OMP_SIMD_H_INCLUDED
