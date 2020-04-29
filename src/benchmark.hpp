#ifndef BENCHMARK_H_INCLUDED
#define BENCHMARK_H_INCLUDED

/**
 * \file     benchmark.hpp
 * \mainpage Functions for benchmarking performance
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

#include "align.hpp"
#include "timer.hpp"
#include "omp_simd.hpp"
#include "avx_omp.hpp"


/**\fn        test_alignment
 * \brief     Test cache alignment of the \p first element of a container as well
 *            as if its \p length is a multiple of the cache line. It contains the
 *            leftover bytes. For ideal performance the result should be 0 for
 *            both
 *
 * \param[in] first element   pointer to first element
 * \param[in] length          size of the corresponding container
*/
template <typename T>
void test_alignment(T* const first_element, size_t const length)
{
    std::cout.setf(std::ios::boolalpha);
    std::cout << " first_element%cache_line: " << (size_t)(first_element) % CACHE_LINE  << std::endl;
    std::cout << " length%cache_line:        " << (length*sizeof(INTR)) % CACHE_LINE    << std::endl;
}


/**\fn    ignore_unused
 * \brief Dummy function that makes the compiler ignore that a \p variable
 *        (e.g. in a benchmark function) is not used in the following code.
*/
template<typename T>
inline void ignore_unused(const T&)
{
}


/**\fn        benchmark_fun
 * \brief     Function that benchmarks the performance of a function \p f that is
 *            operating on contiguous standard C++ containers by calling it \it
 *            times in a row.
 *
 * \param[in] x   a (un)aligned C++ or boosted aligned std vector/array
 * \param[in] y   a (un)aligned C++ or boosted aligned std vector/array
 * \param[in] f   pointer to a function that takes two references to vectors/arrays as
 *                arguments
 * \param[in] it  number of iterations for test
*/
template <typename T1 = std::span<double>, typename T2 = double>
void benchmark_fun(T1 const &x, T1 const &y, T2 (*f)(T1 const &x, T1 const &y), size_t it)
{
    typedef typename T1::value_type ded_type;
    static_assert(std::is_same<ded_type,T2>::value == true);

    Timer stopwatch;
    stopwatch.Start();

    for (size_t i = 0; i < it; ++i)
    {
        INTR volatile res = (*f)(x, y);
        ignore_unused(res);
    }

    stopwatch.Stop();
    std::cout << " runtime: " << stopwatch.GetRuntime() << ", result: " << (*f)(x, y) << std::endl;
}

#endif // BENCHMARK_H_INCLUDED
