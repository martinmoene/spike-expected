//
// expected - variations on Expected<T> by Andrei Alexandrescu.
//
// Written in 2013 by Martin Moene <martin.moene@gmail.com>
//

#ifndef STD_EXCEPTION_NONCPP11_H_INCLUDED
#define STD_EXCEPTION_NONCPP11_H_INCLUDED

#if __cplusplus < 201103L

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

#endif // STD_EXCEPTION_NONCPP11_H_INCLUDED
