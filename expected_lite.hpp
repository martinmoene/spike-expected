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

#include <cassert>      // assert()
#include <functional>   // std::less<>
#include <new>          // placement new
#include <stdexcept>    // std:logic_error
#include <utility>      // std::swap()

#if __cplusplus < 201103L       // non-C++11 compilers
# include "exception_c++11.hpp" // std::exception_ptr, std::move()
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

private:
    typedef void (expected::*safe_bool)() const;
    void this_type_does_not_support_comparisons() const {}

    bool initialized() const
    {
        return has_value;
    }

    void clear()
    {
        if ( initialized() )
            contained.destruct_value();

        has_value = false;
    }

    template <typename V>
    void initialize( V const & v )
    {
        assert( ! has_value );
        contained.construct_value( v );
        has_value = true;
    }

public:
    // constructors

    expected()
    : has_value( false )
    {
        contained.construct_error( std::exception_ptr() );
    }

    expected( nullexp_t )
    : has_value( false )
    {
        contained.construct_error( std::exception_ptr() );
    }

    expected( value_type const & rhs )
    : has_value( true )
    {
        contained.construct_value( rhs );
    }

    expected( nullexp_t, error_type const & rhs )
    : has_value( false )
    {
        contained.construct_error( rhs );
    }

    expected( expected const & rhs )
    : has_value( rhs.has_value )
    {
        if ( has_value ) contained.construct_value( rhs.contained.value() );
        else             contained.construct_error( rhs.contained.error() );
    }

    // destructor

    ~expected()
    {
        if ( has_value ) contained.destruct_value();
        else             contained.destruct_error();
    }

    // assignment

    expected & operator =( nullexp_t )
    {
        clear();
        return *this;
    }

    expected & operator =( expected const & rhs )
    {
        if      ( initialized() == true  && rhs.initialized() == false ) clear();
        else if ( initialized() == false && rhs.initialized() == true  ) initialize(*rhs);
        else if ( initialized() == true  && rhs.initialized() == true  ) contained.value() = *rhs;
        return *this;
    }

    // swap

    void swap( expected & rhs )
    {
        using std::swap;

        if      ( initialized() == true  && rhs.initialized() == false ) { rhs.initialize(**this); clear(); }
        else if ( initialized() == false && rhs.initialized() == true  ) { initialize(*rhs);   rhs.clear(); }
        else if ( initialized() == true  && rhs.initialized() == true  ) { swap(**this, *rhs);              }

#if 0
        if ( has_value )
        {
            if ( rhs.has_value )
            {
                swap( contained.value(), rhs.contained.value() );
            }
            else
            {
                error_type t = rhs.contained.error();

                rhs.contained.construct_value( value() );
                contained.construct_error( t );

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
                swap( contained.error(), rhs.contained.error() );
            }
        }
#endif
    }

    // observers

    value_type const * operator ->() const
    {
        assert( has_value );
        return contained.value_ptr();
    }

    value_type * operator ->()
    {
        assert( has_value );
        return contained.value_ptr();
    }

    value_type const & operator *() const
    {
        assert( has_value );
        return contained.value();
    }

    value_type & operator *()
    {
        assert( has_value );
        return contained.value();
    }

    operator safe_bool() const
    {
        return has_value ? &expected::this_type_does_not_support_comparisons : 0;
    }

    value_type const & value() const
    {
        return has_value ? contained.value() : ( std::rethrow_exception( contained.error() ), contained.value() );
    }

    value_type & value()
    {
        return has_value ? contained.value() : ( std::rethrow_exception( contained.error() ), contained.value() );
    }

    template <typename V>
    value_type value_or( V const & v ) const
    {
        return has_value ? **this : v;
    }

    error_type const & error() const
    {
        return has_value ? (throw nonstd::bad_expected_access("expected: no contained error"), contained.error() ) : contained.error();
    }

private:
    bool has_value;
    storage_t<T,E> contained;
};

// Relational operators

template <typename T, typename E>
bool operator==( expected<T,E> const & x, expected<T,E> const & y )
{
  return bool(x) != bool(y) ? false : bool(x) == false ? true : *x == *y;
}

template <typename T, typename E>
bool operator!=( expected<T,E> const & x, expected<T,E> const & y )
{
  return !(x == y);
}

template <typename T, typename E>
bool operator<( expected<T,E> const & x, expected<T,E> const & y )
{
  return (!y) ? false : (!x) ? true : std::less<T>()( *x, *y );
//  return (!y) ? false : (!x) ? true : *x < *y;

}

template <typename T, typename E>
bool operator>( expected<T,E> const & x, expected<T,E> const & y )
{
  return (y < x);
}

template <typename T, typename E>
bool operator<=( expected<T,E> const & x, expected<T,E> const & y )
{
  return !(y < x);
}

template <typename T, typename E>
bool operator>=( expected<T,E> const & x, expected<T,E> const & y )
{
  return !(x < y);
}

// Comparison with nullexp

template <typename T, typename E>
bool operator==( expected<T,E> const & x, nullexp_t )
{
  return (!x);
}

template <typename T, typename E>
bool operator==( nullexp_t, expected<T,E> const & x )
{
  return (!x);
}

template <typename T, typename E>
bool operator!=( expected<T,E> const & x, nullexp_t )
{
  return bool(x);
}

template <typename T, typename E>
bool operator!=( nullexp_t, expected<T,E> const & x )
{
  return bool(x);
}

template <typename T, typename E>
bool operator<( expected<T,E> const &, nullexp_t )
{
  return false;
}

template <typename T, typename E>
bool operator<( nullexp_t, expected<T,E> const & x )
{
  return bool(x);
}

template <typename T, typename E>
bool operator<=( expected<T,E> const & x, nullexp_t )
{
  return (!x);
}

template <typename T, typename E>
bool operator<=( nullexp_t, expected<T,E> const & )
{
  return true;
}

template <typename T, typename E>
bool operator>( expected<T,E> const & x, nullexp_t )
{
  return bool(x);
}

template <typename T, typename E>
bool operator>( nullexp_t, expected<T,E> const & )
{
  return false;
}

template <typename T, typename E>
bool operator>=( const expected<T,E>&, nullexp_t )
{
  return true;
}

template <typename T, typename E>
bool operator>=( nullexp_t, expected<T,E> const & x )
{
  return (!x);
}

// Comparison with T

template <typename T, typename E>
bool operator==( expected<T,E> const & x, T const & v )
{
  return bool(x) ? *x == v : false;
}

template <typename T, typename E>
bool operator==(T const & v, expected<T,E> const & x )
{
  return bool(x) ? v == *x : false;
}

template <typename T, typename E>
bool operator!=( expected<T,E> const & x, T const & v )
{
  return bool(x) ? *x != v : true;
}

template <typename T, typename E>
bool operator!=( T const & v, expected<T,E> const & x )
{
  return bool(x) ? v != *x : true;
}

template <typename T, typename E>
bool operator<( expected<T,E> const & x, T const & v )
{
//  return bool(x) ? *x < v : true;
  return bool(x) ? std::less<T>()( *x, v ) : true;
}

template <typename T, typename E>
bool operator>( T const & v, expected<T,E> const & x )
{
  return bool(x) ? std::greater<T>()( v, *x ) : true;
//  return bool(x) ? v > *x : true;
}

template <typename T, typename E>
bool operator>( expected<T,E> const & x, T const & v )
{
  return bool(x) ? std::greater<T>()( *x, v ) : false;
//  return bool(x) ? *x > v : false;
}

template <typename T, typename E>
bool operator<( T const & v, expected<T,E> const & x )
{
  return bool(x) ? std::less<T>()( v, *x ) : false;
//  return bool(x) ? v < *x : false;
}

template <typename T, typename E>
bool operator>=( expected<T,E> const & x, T const & v )
{
  return bool(x) ? std::greater_equal<T>()( *x, v ): false;
//  return bool(x) ? *x >= v : false;
}

template <typename T, typename E>
bool operator<=( T const & v, expected<T,E> const & x )
{
  return bool(x) ? std::less_equal<T>()( v, *x ) : false;
//  return bool(x) ? v <= *x : false;
}

template <typename T, typename E>
bool operator<=( expected<T,E> const & x, T const & v )
{
  return bool(x) ? std::less_equal<T>()( *x, v ) : true;
//  return bool(x) ? *x <= v : true;
}

template <typename T, typename E>
bool operator>=( T const & v, expected<T,E> const & x )
{
  return bool(x) ? std::less_equal<T>()( *x, v ) : true;
//  return bool(x) ? *x <= v : true;
}

// Specialized algorithms

template <typename T, typename E>
void swap( expected<T,E> & x, expected<T,E> & y)
{
  x.swap( y );
}

// template <typename T, typename E> expected<see below> make_optional(T&&);

// hash support
//template <typename T, typename E> struct hash;
//template <typename T, typename E> struct hash<expected<T,E> >;

} // namespace nonstd

#endif // NONSTD_EXPECTED_LITE_HPP
