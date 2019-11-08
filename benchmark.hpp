#ifndef BENCHMARK_H_INCLUDED
#define BENCHMARK_H_INCLUDED

/**
 * \file     benchmark.hpp
 * \mainpage Functions for benchmarking performance
*/


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
 * \param[in] first element: pointer to first element
 * \param[in] length: size of the corresponding container
*/
template <typename T>
void test_alignment(T* const first_element, size_t const length)
{
    std::cout.setf(std::ios::boolalpha);
    std::cout << " first_element%cache_line: " << (size_t)(first_element) % CACHE_LINE  << std::endl;
    std::cout << " length%cache_line: "        << (length*sizeof(INTR)) % CACHE_LINE    << std::endl;
}


/**\fn    ignore_unused
 * \brief Dummy function that makes the compiler ignore that a \p variable
 *        (e.g. in a benchmark function) is not used in the following code.
*/
template<typename T>
inline void ignore_unused(const T&)
{
}


/**\fn        benchmark_vec
 * \brief     Function that benchmarks the performance of a function \p f that is
 *            operating on (boost aligned) C++ std vectors by calling it \it
 *            times in a row.
 *
 * \param[in] x: a (un)aligned C++ or boosted aligned std vector
 * \param[in] y: a (un)aligned C++ or boosted aligned std vector
 * \param[in] f: pointer to a function that takes two references to vectors as
 *               arguments
 * \param[in] it: number of iterations for test
*/
template <typename T = double>
void benchmark_vec(VEC(T) const &x, VEC(T) const &y, T (*f)(VEC(T) const &x, VEC(T) const &y), size_t it)
{
    Timer stopwatch;
    stopwatch.Start();

    for (size_t i = 0; i < it; ++i)
    {
        INTR res = (*f)(x, y); //call function
        ignore_unused(res);    //ignore compiler warning
    }

    stopwatch.Stop();
    std::cout << " runtime: " << stopwatch.GetRuntime() << ", result: " << (*f)(x, y) << std::endl;
}


/**\fn        benchmark_arr
 * \brief     Function that benchmarks the performance of a function \p f that is
 *            operating on C arrays by calling it \it times in a row.
 *
 * \param[in] x: a (un)aligned array
 * \param[in] y: a (un)aligned array
 * \param[in] f: pointer to a function that takes two references to arrays as
 *               arguments
 * \param[in] it: number of iterations for test
*/
template <typename T = double, size_t N>
void benchmark_arr(T const (&x)[N], T const (&y)[N], T (*f)(T const (&x)[N], T const (&y)[N]), size_t it)
{
    Timer stopwatch;
    stopwatch.Start();

    for (size_t i = 0; i < it; ++i)
    {
        INTR res = (*f)(x, y); //call function
        ignore_unused(res);    //ignore compiler warning
    }

    stopwatch.Stop();
    std::cout << " runtime: " << stopwatch.GetRuntime() << ", result: " << (*f)(x, y) << std::endl;
}

#endif // BENCHMARK_H_INCLUDED
