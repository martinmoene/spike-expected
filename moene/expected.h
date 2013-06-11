//
// expected - variations on Expected<T> by Andrei Alexandrescu.
//
// Written in 2013 by Martin Moene <martin.moene@gmail.com>
//

#ifndef NONSTD_EXPECTED_MOENE_H_INCLUDED
#define NONSTD_EXPECTED_MOENE_H_INCLUDED

#include <algorithm>            // std::move()
#include <exception>            // std::exception_ptr

#if __cplusplus < 201103L       // non-C++11 compilers
# include "exception_c++11.h"   // std::exception_ptr, std::move()
#endif

namespace std11
{

struct exceptional_tag {} exceptional;

template <typename ValueType, typename ExceptType = std::exception_ptr>
class expected
{
public:
    typedef ValueType value_type;
    typedef ExceptType error_type;

private:
    typedef expected this_type;

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

public:
    // constructors

    expected( value_type value )
    : m_value( value )
    , m_valid( true  )
    {}

    expected( exceptional_tag, error_type const & error )
    : m_error( error )
    , m_valid( false )
    {}

    expected( expected const & other )
    : m_valid( other.m_valid )
    {
        if ( m_valid )
        {
            new( &m_value ) value_type( other.m_value );
        }
        else
        {
            new( &m_error ) error_type( other.m_error );
        }
    };

    // destructor

    ~expected()
    {
#if __cplusplus >= 201103L
        if ( valid() )
            m_value.~value_type();
        else
            m_error.~error_type();
#endif
    }

    // assignment

    expected & operator= ( expected e )
    {
        e.swap( *this );
        return *this;
    }

    // swap

    void swap( expected & rhs )
    {
        using std::swap;

        if ( m_valid )
        {
            if ( rhs.m_valid )
            {
                swap( m_value, rhs.m_value );
            }
            else
            {
                error_type t = std::move( rhs.m_error );

                new ( &rhs.m_value) value_type( std::move( m_value ) );
                new ( &m_error    ) error_type( t );

                swap( m_valid, rhs.m_valid );
            }
        }
        else
        {
            if ( rhs.m_valid )
            {
                rhs.swap( *this );
            }
            else
            {
                swap( m_error, rhs.m_error );
            }
        }
    }

    // observers

#if 0

    constexpr T const* operator ->() const;
    T* operator ->();
    constexpr T const& operator *() const;
    T& operator *();
    constexpr explicit operator bool() const noexcept;
    constexpr T const& value() const;
    T& value();
    template <class U> constexpr T value_or(U&&) const&;
    template <class U> T value_or(U&&) &&;

#endif

    bool valid() const
    {
        return m_valid;
    }

#if __cplusplus >= 201103L
    explicit operator bool() const
#else
    // operator safe_bool()
    operator bool() const
#endif
    {
        return valid();
    }

    value_type const & value() const
    {
        if ( !valid() )
        {
            std::rethrow_exception( m_error );
        }
        return m_value;
    }

    value_type & value()
    {
        if ( !valid() )
        {
            std::rethrow_exception( m_error );
        }
        return m_value;
    }

    value_type const & operator* () const
    {
        return m_value;
    }

    value_type & operator* ()
    {
        return m_value;
    }

    // -------------------------------------------------------------------

    // Utilities:

    template <typename Visitor>
    expected<typename Visitor::result_type, error_type>
    apply( Visitor visit ) const
    {
        typedef typename Visitor::result_type result_value_type;

        if ( valid() )
        {
            return expected<result_value_type, error_type>( visit( m_value ) );
        }
        else
        {
            return expected<result_value_type, error_type>( exceptional, m_error );
        }
    }

    template <typename Handler>
    expected<typename Handler::result_type, error_type>
    handle( Handler visit )
    {
        return visit( *this );
    }
};

#if 0

// 20.5.8, Relational operators
template <class T> constexpr bool operator==(const expected<T>&, const expected<T>&);
template <class T> constexpr bool operator<(const expected<T>&, const expected<T>&);

// 20.5.9, Comparison with nullexp
template <class T> constexpr bool operator==(const expected<T>&, nullexp_t) noexcept;
template <class T> constexpr bool operator==(nullexp_t, const expected<T>&) noexcept;
template <class T> constexpr bool operator<(const expected<T>&, nullexp_t) noexcept;
template <class T> constexpr bool operator<(nullexp_t, const expected<T>&) noexcept;

// 20.5.10, Comparison with T
template <class T> constexpr bool operator==(const expected<T>&, const T&);
template <class T> constexpr bool operator==(const T&, const expected<T>&);
template <class T> constexpr bool operator<(const expected<T>&, const T&);
template <class T> constexpr bool operator<(const T&, const expected<T>&);

// 20.5.11, Specialized algorithms
template <class T> void swap(expected<T>&, expected<T>&) noexcept(see below);
template <class T> constexpr expected<see below> make_expected(T&&);

// 20.5.12, hash support
template <class T> struct hash;
template <class T> struct hash<expected<T> >;

#endif

// Factories:

template <typename T, typename E>
expected<T, E>
make_expected( T const & rhs )
{
    return expected<T, E>( rhs );
}

template <typename T, typename E>
expected<T, E>
make_exceptional_expected( E const & e )
{
    return expected<T, E>( exceptional, e );
}


} // namespace std11

#endif // NONSTD_EXPECTED_MOENE_H_INCLUDED
