#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

/**
 * \file     init.hpp
 * \mainpage Functions that initialise a vector and then an array with random
 *           numbers.
 * \warning  Initialise vector first and then copy the values to an array.
*/


#include <assert.h>
#include <vector>
#include "align.hpp"


/**\fn        init_vec
 * \brief     Initialise vecor of size \p length with random numbers
 *
 * \param[in] length: length of the wished vector
 * \return    A vector of size \p length and random entries between 0 and 1
*/
template <typename T = double>
VEC(T) init_vec(size_t const length)
{
    assert(length > 0);

    VEC(T) res(length,0);

    for (size_t i = 0; i < res.size(); ++i)
    {
        res[i] = static_cast<T>(std::rand())/static_cast<T>(RAND_MAX);
    }

    return res;
}


/**\fn         vec_to_arr
 * \brief      Copy values from the vector \p vec to the array \p arr
 *
 * \param[in]  vec: C++ std vector filled with numbers
 * \param[out] arr: preferably empty C array
*/
template <typename T, size_t N>
void vec_to_arr(VEC(T) const vec, T (&arr)[N])
{
    // check if size is compatible
    assert(vec.size() <= N);

    for (size_t i = 0; i < vec.size(); ++i)
    {
        arr[i] = vec[i];
    }
}

#endif // INIT_H_INCLUDED
