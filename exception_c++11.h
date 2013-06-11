//
// expected - variations on Expected<T> by Andrei Alexandrescu.
//
// Written in 2013 by Martin Moene <martin.moene@gmail.com>
//

#ifndef STD_EXCEPTION_CPP11_H_INCLUDED
#define STD_EXCEPTION_CPP11_H_INCLUDED

// VC2010+
#if defined( _MSC_VER ) && ( _MSC_VER >= 1600 )
# define COMPILER_IS_MSVC2010
# define HAS_STD_EXCEPTION_PTR
#endif

#if __cplusplus < 201103L && ! defined( HAS_STD_EXCEPTION_PTR )

#include "exception_ptr_lite.h"

namespace std {

using std11::exception_ptr;
using std11::make_exception_ptr;
using std11::rethrow_exception;
using std11::current_exception;

template <typename T>
inline T & move( T & rhs )
{
    return rhs;
}

} // namespace std

#endif // not C++11


// Special cases:

#ifdef COMPILER_IS_MSVC2010
namespace std {

template <typename E>
inline exception_ptr
make_exception_ptr( E const & e )
{
    try
    {
        throw e;
    }
    catch (...)
    {
        return current_exception();
    }
}

} // namespace std

#endif // COMPILER_IS_MSVC2010

#undef HAS_STD_EXCEPTION_PTR
#undef COMPILER_IS_MSVC2010

#endif // STD_EXCEPTION_CPP11_H_INCLUDED
