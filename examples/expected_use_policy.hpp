// Copyright (C) 2013 Martin Moene.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NONSTD_EXPECTED_USE_POLICY_HPP_INCLUDED
#define NONSTD_EXPECTED_USE_POLICY_HPP_INCLUDED

#include <stdexcept>

struct nullexp_t {} nullexp;

/**
 * exception on error access when no error is contained.
 */
class bad_expected_access : public std::logic_error
{
public:
  explicit bad_expected_access( const char * what_arg )
  : logic_error( what_arg ) {}
};

struct ignore_policy
{
    void use() {}
};

struct require_policy
{
    bool ignored = true;

    ~require_policy() noexcept(false)
    {
        if ( ignored )
            throw bad_expected_access("expected: value ignored");
    }

    void use()
    {
        ignored = false;
    }
};

template <typename T, typename Use = ignore_policy>
class expected
{
    typedef std::exception_ptr E;

public:
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

    ~expected() noexcept(false)
    {
        if ( has_value )
        {
            contained_value.~T();
        }
        else
        {
            contained_error.~E();
        }
    }

    T const & value() const
    {
        if ( ! has_value )
            std::rethrow_exception( contained_error );

        use_value.use();

        return contained_value;
    }

private:
    union
    {
        T contained_value;
        E contained_error;
    };
    bool has_value;
    Use use_value;
};

#endif // NONSTD_EXPECTED_USE_POLICY_HPP_INCLUDED
