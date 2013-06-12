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

#ifndef NONSTD_EXPECTED_LITE_HPP
#define NONSTD_EXPECTED_LITE_HPP

#include <new>          // placement new
#include <stdexcept>    // std:logic_error
#include <utility>      // std::swap()

#if __cplusplus < 201103L       // non-C++11 compilers
# include "exception_c++11.h"   // std::exception_ptr, std::move()
#endif

namespace nonstd {

#define NONSTD_EXPECTED_LITE_MAX( a, b ) ( (a) > (b) ? a : b )

template <typename T, typename E>
class expected;

/**
 * exception on error access when no error is contained.
 */
class bad_expected_access : public std::logic_error
{
public:
  explicit bad_expected_access( const char * what_arg )
  : logic_error( what_arg ) {}
};

/**
 * C++03 constructed union to hold value or error.
 */
template <typename T, typename E>
union storage_t
{
    friend class expected<T,E>;

private:
    typedef T value_type;
    typedef E error_type;

    void construct_value( value_type const & v )
    {
        new( value_ptr() ) value_type( v );
    }

    void destruct_value()
    {
        // Note: VC6 requires the use of the template parameter T
        value_ptr()->~T();
    }

    void construct_error( error_type const & e )
    {
        new( error_ptr() ) error_type( e );
    }

    void destruct_error()
    {
        // Note: VC6 requires the use of the template parameter T
        error_ptr()->~E();
    }

    value_type const & value() const
    {
        return * value_ptr();
    }

    value_type & value()
    {
        return * value_ptr();
    }

    error_type const & error() const
    {
        return * error_ptr();
    }

    error_type & error()
    {
        return * error_ptr();
    }

    value_type * value_ptr() const
    {
        return as( (value_type*)0 );
    }

    error_type * error_ptr() const
    {
        return as( (error_type *)0 );
    }

    // Note: VC6 cannot handle as<T>():

    template <typename U>
    U * as( U* ) const
    {
        return reinterpret_cast<U*>( const_cast<unsigned char *>( buffer ) );
    }

    unsigned char dummy;
    unsigned char buffer[ NONSTD_EXPECTED_LITE_MAX( sizeof(T), sizeof(E) ) ];
};

#undef NONSTD_EXPECTED_LITE_MAX

const struct nullexp_t {} nullexp;
// const struct exceptional_t {} exceptional;

/**
 * expected value or error.
 */
template <typename T, typename E = std::exception_ptr>
class expected
{
public:
    typedef T value_type;
    typedef E error_type;

    // constructors

    expected( value_type const & rhs )
    : has_value( true )
    {
        storage.construct_value( rhs );
    }

    expected( nullexp_t, error_type const & rhs )
    : has_value( false )
    {
        storage.construct_error( rhs );
    }

    expected( expected const & rhs )
    : has_value( rhs.has_value )
    {
        if ( has_value ) storage.construct_value( rhs.storage.value() );
        else             storage.construct_error( rhs.storage.error() );
    }

    // destructor

    ~expected()
    {
        if ( has_value ) storage.destruct_value();
        else             storage.destruct_error();
    }

    // assignment

#ifdef NONSTD_EXPECTED_DEFINE_ASSIGNMENT

    expected & operator =( expected rhs )
    {
        rhs.swap( *this );
        return *this;
    }
#else
    private: expected & operator =( expected rhs );
    public:
#endif // NONSTD_EXPECTED_DEFINE_ASSIGNMENT

    // swap

    void swap( expected & rhs )
    {
        using std::swap;

        if ( has_value )
        {
            if ( rhs.has_value )
            {
                swap( value(), rhs.value() );
            }
            else
            {
                error_type t = rhs.storage.error();

                rhs.storage.construct_value( value() );
                storage.construct_error( t );

                swap( has_value, rhs.has_value );
            }
        }
        else
        {
            if ( rhs.has_value )
            {
                rhs.swap( *this );
            }
            else
            {
                swap( storage.error(), rhs.storage.error() );
            }
        }
    }

    // observers

    value_type const * operator ->() const
    {
        return has_value ? storage.value_ptr() : ( std::rethrow_exception( storage.error() ), storage.value_ptr() );
    }

    value_type * operator ->()
    {
        return has_value ? storage.value_ptr() : ( std::rethrow_exception( storage.error() ), storage.value_ptr() );
    }

    value_type const & operator *() const
    {
        return has_value ? * storage.value_ptr() : ( std::rethrow_exception( storage.error() ), * storage.value_ptr() );
    }

    value_type & operator *()
    {
        return has_value ? * storage.value_ptr() : ( std::rethrow_exception( storage.error() ), * storage.value_ptr() );
    }

    /*explicit*/ operator bool() const
    {
        return has_value;
    }

    value_type const & value() const
    {
        return **this;
    }

    value_type & value()
    {
        return **this;
    }

    template <typename V>
    value_type value_or( V const & v ) const
    {
        return *this ? **this : v;
    }

    error_type const & error() const
    {
        return has_value ? (throw nonstd::bad_expected_access("expected: no contained error"), storage.error() ) : storage.error();
    }

private:
    bool has_value;
    storage_t<T,E> storage;
};

} // namespace nonstd

#endif // NONSTD_EXPECTED_LITE_HPP
