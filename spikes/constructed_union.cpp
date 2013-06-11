//
// expected - variations on Expected<T> by Andrei Alexandrescu.
//
// Written in 2013 by Martin Moene <martin.moene@gmail.com>
//

#include <iostream>

#ifndef STD11_CONSTRUCTED_UNION_H_INCLUDED
#define STD11_CONSTRUCTED_UNION_H_INCLUDED

#include <utility>  // std::forward()

struct in_place_t {} in_place;

template <typename T, typename E>
class expected;

#if __cplusplus >= 201103L  // C++11 and later

template <typename T, typename E>
union storage_t
{
    friend class expected<T,E>;

    typedef T value_type;
    typedef E error_type;

    void emplace_value( value_type && v )
    {
        new ( value_ptr() ) value_type( std::forward<value_type>( v ) );
    }

    void construct_value( value_type const & v )
    {
        new( value_ptr() ) value_type( v );
    }

    void destruct_value()
    {
        value_.~value_type();
    }

    void emplace_error( error_type && e )
    {
        new ( error_ptr() ) error_type( std::forward<error_type>( e ) );
    }

    void construct_error( error_type const & e )
    {
        new( error_ptr() ) error_type( e );
    }

    void destruct_error()
    {
        error_.~error_type();
    }

    value_type const & value() const
    {
        return value_;
    }

    error_type const & error() const
    {
        return error_;
    }

    storage_t() {}
    ~storage_t() {}

private:
    value_type * value_ptr()
    {
        return std::addressof( value_ );
    }

    error_type * error_ptr()
    {
        return std::addressof( error_ );
    }

    value_type value_;
    error_type error_;
};

#else // not C++11:

#define stdmax( a, b ) ( (a) > (b) ? a : b )

template <typename T, typename E>
union storage_t
{
    typedef T value_type;
    typedef E error_type;

#if HAS_RVALUEREFERENCE

    void emplace_value( value_type && v )
    {
        new ( value_ptr() ) value_type( v );
    }
#endif

    void construct_value( value_type const & v )
    {
        new( value_ptr() ) value_type( v );
    }

    void destruct_value()
    {
        // Note: VC6 requires the use of the template parameter T
        value_ptr()->~T();
    }

#if HAS_RVALUEREFERENCE

    void emplace_error( error_type && e )
    {
        new ( error_ptr() ) error_type( e );
    }
#endif

    void construct_error( error_type const & e )
    {
        new( error_ptr() ) error_type( e );
    }

    void destruct_error()
    {
        // Note: VC6 requires the use of the template parameter T
        error_ptr()->~E();
    }

    value_type const & value()
    {
        return * value_ptr();
    }

    error_type const & error()
    {
        return * error_ptr();
    }

private:
    value_type * value_ptr()
    {
        return as( (value_type*)0 );
    }

    error_type * error_ptr()
    {
        return as( (error_type *)0 );
    }

    // Note: VC6 cannot handle as<T>():

    template <typename U>
    U * as( U* )
    {
        return reinterpret_cast<U*>( buffer );
    }

    int dummy;
    unsigned char buffer[ stdmax( sizeof(T), sizeof(E)) ];
};

#undef stdmax

#endif // C++11

#endif // STD11_CONSTRUCTED_UNION_H_INCLUDED


struct std_exception_ptr
{
    std_exception_ptr()           { std::cout << "std_exception_ptr::std_exception_ptr()" << std::endl; }
    std_exception_ptr(std_exception_ptr const & ) { std::cout << "std_exception_ptr::std_exception_ptr(std_exception_ptr const &)" << std::endl; }
    ~std_exception_ptr()          { std::cout << "std_exception_ptr::~std_exception_ptr()" << std::endl; }

};


struct exceptional_t {} exceptional;

template <typename T, typename E = std_exception_ptr>
class expected
{
    typedef T value_type;
    typedef E error_type;

    storage_t<T,E> storage;
    bool has_value;

public:
    expected( value_type const & v )
    : has_value( true )
    {
        storage.construct_value( v );
    }

    expected( exceptional_t )
    : has_value( false )
    {
        storage.construct_error( error_type() );
    }

    expected( expected const & rhs )
    : has_value( rhs.has_value )
    {
        if ( rhs.has_value ) storage.construct_value( rhs.storage.value() );
        else                 storage.construct_error( rhs.storage.error() );
    }

#if __cplusplus >= 201103L

    expected( expected && rhs )
    : has_value( rhs.has_value )
    {
        if ( rhs.has_value ) storage.emplace_value( std::forward<value_type>( rhs.storage.value_ ) );
        else                 storage.emplace_error( std::forward<error_type>( rhs.storage.error_ ) );
    }

    expected emplace( value_type && v )
    {
        has_value = true;

        storage.emplace_value( std::forward<value_type>( v ) );

        return *this;
    }
#endif

    ~expected()
    {
        if ( has_value ) storage.destruct_value();
        else             storage.destruct_error();
    }

#if __cplusplus >= 201103L

    explicit operator bool() const
    {
        return has_value;
    }

#else
    // NTS:safe_bool
    operator bool() const
    {
        return has_value;
    }

#endif

    value_type const & value() // const
    {
//        if ( has_value )
            return storage.value();
    }

    value_type const & operator*() // const
    {
        return value();
    }

#if 0 // __cplusplus >= 201103L
    template <class V>
    constexpr T value_or( V&& v ) const
    {
        return *this ? **this : static_cast<T>( constexpr_forward<V>(v) );
    }
#else
    template <class V>
    value_type value_or( V const & v ) // const
    {
        return *this ? **this : v;
    }
#endif
};

struct S
{
    S()           { std::cout << "S::S()" << std::endl; }
    S(S const & ) { std::cout << "S::S(S const &)" << std::endl; }

#if __cplusplus >= 201103L
    S(S &&      ) { std::cout << "S::S(S &&)" << std::endl; }
#endif

    ~S()          { std::cout << "S::~S()" << std::endl; }
};


int main()
{
    expected<int> ei = 33;
    std::cout << "expected<int> ei = 33: " << ei.value() << std::endl;

    std::cout << "ei has value: " << (ei ? "yes" : "no") << std::endl;

    S s;
    expected<S> es ( s );

#if __cplusplus >= 201103L
    es.emplace( std::forward<S>( S() ) );
#endif
    expected<int> ee = expected<int>( exceptional );

    std::cout << "ee has value: " << (ee ? "yes" : "no") << std::endl;

    std::cout << "ee.value_or(77): " << ee.value_or(77) << std::endl;

    return 0; // VC6
}

// cl -nologo -W3 -EHsc constructed_union.cpp && constructed_union
// g++ -Wall -Wextra -std=c++11 -o constructed_union.exe constructed_union.cpp && constructed_union
