#ifndef CONSTEXPR_FUNC_H_INCLUDED
#define CONSTEXPR_FUNC_H_INCLUDED

/**
 * \file     constexpr_func.hpp
 * \mainpage A series of functions that can evaluate to a constant expression and thus
 *           can make use of evaluation and optimisation at compile time.
*/


#include <limits>


namespace cef
{
    /**\fn        cef::sqrt
     * \brief     Square root implementation with recursive Newton-Raphson method that can
     *            be evaluated as a constant expression at compile time
     *
     * \param[in] x      the number of interest
     * \param[in] curr   the result from the current iteration
     * \param[in] prev   the result from the previous iteration
     * \return    the square root of \param x
    */
    template <typename T = double>
    T constexpr sqrtNewton(T const x, T const curr, T const prev)
    {
        return curr == prev
               ? curr
               : sqrtNewton(x, static_cast<T>(0.5 * (curr + x / static_cast<double>(curr))), curr);
    }

    template <typename T = double>
    T constexpr sqrt(T const x)
    {
        return x >= static_cast<T>(0.0) && x < std::numeric_limits<double>::infinity()
               ? sqrtNewton(x, x, static_cast<T>(0.0))
               : std::numeric_limits<double>::quiet_NaN();
    }
}

#endif // CONSTEXPR_FUNC_H_INCLUDED
