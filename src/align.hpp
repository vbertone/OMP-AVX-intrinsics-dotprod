#ifndef ALIGN_H_INCLUDED
#define ALIGN_H_INCLUDED

/**
 * \file     align.hpp
 * \mainpage File for setting cache related settings
*/


/// size of the cache line of the architecture (typically 64 bit)
#define CACHE_LINE    64

/// define type to be used for arrays and vectors (must match the intrinsic data type!)
typedef double        INTR;

/// macro for padding
#define PAD(N,T)      ((CACHE_LINE - sizeof(T)*N % CACHE_LINE) % CACHE_LINE) / sizeof(T)

/// turn boost library aligned allocator on and off with variadic macro
#if __has_include (<boost/align/aligned_allocator.hpp>)
    #include <boost/align/aligned_allocator.hpp>
    #include <boost/version.hpp>
    #define VEC(T) std::vector<T, boost::alignment::aligned_allocator<T, CACHE_LINE>>
#else
    #define VEC(T) std::vector<T>
#endif

#endif // ALIGN_H_INCLUDED
