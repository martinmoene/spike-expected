// Copyright (C) 2013 Martin Moene.
//
// Implementation note: this version targets C++03.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// The idea is based on the Expected<T> class template authored by
// Andrei Alexandrescu. The interface is based on the std::optional proposal
// authored by Fernando Cacciola and Andrzej Krzemienski and the
// Boost.Expected proposal authored by Pierre Talbot.

#ifndef NONSTD_EXPECTED_HPP
#define NONSTD_EXPECTED_HPP

#include <exception>

#if __cplusplus < 201103L       // non-C++11 compilers
# include "exception_c++11.h"   // std::exception_ptr, std::move()
#endif

namespace nonstd
{

const struct nullexp_t {} nullexp;

const struct only_set_initialized_t{} only_set_initialized{};


template <typename T, typename E>
struct expected_base
{
    typedef T value_type;
    typedef E error_type;

#if __cplusplus >= 201103L
    union
    {
        value_type m_value;
        error_type m_error;
    };
#else
    value_type m_value;
    error_type m_error;
#endif
    bool m_valid;

    expected_base( value_type const & rhs )
    : m_value( rhs ), m_valid( true ) {}

    expected_base( nullexp_t, error_type err )
    : m_error( err ), m_valid( false ) {}

    expected_base( only_set_initialized, bool b )
    : m_valid( b ) {}
};

template <typename T, typename E = std::exception_ptr>
class expected : private expected_base<T,E>
{
public:
    typedef T value_type;
    typedef E error_type;

    expected( value_type const & rhs )
    : expected_base<T,E>( rhs ) {}

    expected( nullexp_t, error_type const & err )
    : expected_base<T,E>( nullexp, err ) {}

    expected( expected const & rhs )
    : expected_base<T,E>( only_set_initialized, rhs.m_valid )
    {
        if ( rhs.initialized() ) new ( value_ptr() ) T( *rhs );
        else                     new ( error_ptr() ) E( rhs.error )
    }

    ~expected() {}

private:
};

} // namespace nonstd

#endif // NONSTD_EXPECTED_HPP
