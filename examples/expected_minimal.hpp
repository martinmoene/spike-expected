// Copyright (C) 2013 Martin Moene.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NONSTD_EXPECTED_MINIMAL_HPP_INCLUDED
#define NONSTD_EXPECTED_MINIMAL_HPP_INCLUDED

#include <exception>
#include <new>

struct nullexp_t {} nullexp;

template <typename T>
class expected
{
    typedef std::exception_ptr E;

public:
    expected()
    : has_value( false )
    {
        new ( & contained_error ) E( std::exception_ptr() );
    }

    expected( T const & v )
    : has_value( true )
    {
        new ( & contained_value ) T( v );
    }

    expected( nullexp_t, E const & e )
    : has_value( false )
    {
        new ( & contained_error ) E( e );
    }

    ~expected()
    {
        if ( has_value )
            contained_value.~T();
        else
            contained_error.~E();
    }

    explicit operator bool() const
    {
        return has_value;
    }

    T const & value() const
    {
        if ( ! has_value )
            std::rethrow_exception( contained_error );

        return contained_value;
    }

private:
    union
    {
        T contained_value;
        E contained_error;
    };
    bool has_value;
};

#endif // NONSTD_EXPECTED_MINIMAL_HPP_INCLUDED
