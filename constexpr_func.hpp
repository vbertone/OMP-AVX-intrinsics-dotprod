#ifndef CONSTEXPR_FUNC_H_INCLUDED
#define CONSTEXPR_FUNC_H_INCLUDED

/**
 * \file     constexpr_func.hpp
 * \mainpage A series of functions that can evaluate to a constant expression and thus
 *           can make use of evaluation and optimisation at compile time.
*/


#include <limits>
#include <assert.h>


namespace cef
{
    /**\fn        cef::sqrt
     * \brief     Square root implementation with recursive Newton-Raphson method that can
     *            be evaluated as a constant expression at compile time
     *
     * \param[in] x      the number of interest
     * \param[in] curr   the result from the current iteration
     * \param[in] prev   the result from the previous iteration
     * \return    The square root of \param x
    */
    template <typename T = double>
    T constexpr sqrtNewton(T const x, T const curr, T const prev)
    {
        return curr == prev
               ? curr
               : sqrtNewton(x, static_cast<T>(0.5 * (curr + x / static_cast<long double>(curr))), curr);
    }

    template <typename T = double>
    T constexpr sqrt(T const x)
    {
        assert(std::numeric_limits<T>::is_specialized());

        T constexpr max_inf = std::numeric_limits<T>::has_infinity
                              ? std::numeric_limits<T>::infinity()
                              : std::numeric_limits<T>::max();
        T constexpr ret_NaN = std::numeric_limits<T>::has_quiet_NaN
                              ? std::numeric_limits<T>::quiet_NaN()
                              : std::numeric_limits<T>::min();

        return ((x >= static_cast<T>(0.0)) && (x < max_inf))
               ? sqrtNewton(x, x, static_cast<T>(0.0))
               : ret_NaN;
    }
}

#endif // CONSTEXPR_FUNC_H_INCLUDED
