// Copyright (C) 2013 Martin Moene <martin.moene@gmail.com>
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef STD11_EXCEPTION_CPP11_H_INCLUDED
#define STD11_EXCEPTION_CPP11_H_INCLUDED

// VC2010+
#if defined( _MSC_VER ) && ( _MSC_VER >= 1600 )
# define STD11_EXCPT__HAS_STD_EXCEPTION_PTR
# define STD11_EXCPT__DEFINE_STD_MAKE_EXCEPTION_PTR
#endif

#if __cplusplus < 201103L && ! defined( STD11_EXCPT__HAS_STD_EXCEPTION_PTR )

#include "exception_ptr_lite.hpp"

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

#ifdef STD11_EXCPT__DEFINE_STD_MAKE_EXCEPTION_PTR

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

#endif // STD11_EXCPT__DEFINE_STD_MAKE_EXCEPTION_PTR

#undef STD11_EXCPT__DEFINE_STD_MAKE_EXCEPTION_PTR
#undef STD11_EXCPT__HAS_STD_EXCEPTION_PTR

#endif // STD11_EXCEPTION_CPP11_H_INCLUDED
