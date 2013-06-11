//
// expected - variations on Expected<T> by Andrei Alexandrescu.
//
// Written in 2013 by Martin Moene <martin.moene@gmail.com>
//

#include "test_expected.h"
#include "static_visitor.h"

#include <iostream>
#include <string>
#include <cstdlib>

std::string const &
to_string( std::string const & text )
{
    return text;
}

int to_integer( int value )
{
    return value;
}

int to_integer( std::string const & text )
{
    return static_cast<int>( strtol( text.c_str(), NULL, 0) );
}

std11::expected<int> print( std11::expected<int> const & v )
{
    std::cout << "print: " << v.value() << std::endl;
    return v;
}

std11::expected<int> print_s( std11::expected<int> const & v )
{
    if ( v.valid() )
        std::cout << "print_s: " << v.value() << std::endl;
    else
        std::cout << "print_s: [invalid value]" << std::endl;

    return v;
}

struct printer : public nonstd::static_visitor<int>
{
    template <typename T>
    T operator()( T const value )
    {
        std::cout << "printer: " << value << std::endl;
        return to_integer( value );
    }
};

struct s_printer : public nonstd::static_visitor<std::string>
{
    template <typename T>
    T const & operator()( T const & value )
    {
        std::cout << "printer: " << value << std::endl;
        return to_string( value );
    }
};

struct i_doubler : public nonstd::static_visitor<int>
{
    typedef std11::expected<int> int_type;
    int_type operator()( int_type & element )
    {
        element = 2 * element.value();
        return element;
    }

    typedef std11::expected<std::string> string_type;
    int_type operator()( string_type & element )
    {
        element = element.value() + element.value();
        return to_integer( element.value() );
    }
};

struct solver : public nonstd::static_visitor<int>
{
    typedef std11::expected<int> type;
    type operator()( type & element )
    {
        if ( !element.valid() )
        {
            element = 0;
        }
        return element;
    }
};

struct thrower : public nonstd::static_visitor<int>
{
    typedef std11::expected<int> type;
    int operator()( type & element )
    {
        (void)element.value();
        return 0;
    }
};


//
//expected<int> warn( expected<int>::error_type const e )
//{
//    std::cout << "Warning: " << e << std::endl; return 0;
//}
//
//expected<int> fail( expected<int>::error_type const e )
//{
//    std::cout << "Error: " << e << "; skipping rest" << std::endl;
//    return expected<int>( exceptional, "chain partly skipped " );
//}

struct reader : public nonstd::static_visitor<int>
{
    reader()
    {
        std::cout << "Type a, b, or c." << std::endl;
    }

    typedef std11::expected<int> type;
    type operator()( int const value )
    {
        char chr;
        std::cin >> chr;
        if ( std::string::npos == std::string("abc").find( chr ) )
        {
            std::cout << "invalid character: " << chr << std::endl;
            return std11::expected<int>(
                std11::exceptional, std::make_exception_ptr( std::runtime_error("not a,b,c") ) );
        }
        std::cout << "valid character: " << chr << std::endl;
        return chr;
    }
};

int throwing_fun() { throw std::runtime_error( "throwing_fun()" );  }
int nothrowing_fun() { return 4; }

struct throw_always : public nonstd::static_visitor<int>
{
    template <typename T>
    T operator()( T const value )
    {
        throwing_fun();
        return value;
    }
};



int main()
{
    try
    {
        std11::expected<int> err(
            std11::exceptional, std::make_exception_ptr( std::runtime_error("err") ) );

        err.apply( printer() );
    }
    catch ( std::exception const & e )
    {
        std::cout << "main: unhandled error: '" << e.what() << "'" << std::endl;
    }

    try
    {
        std11::expected<int> err(
            std11::exceptional, std::make_exception_ptr( std::runtime_error("err") ) );

        err.apply( printer() );
    }
    catch ( std::exception const & e )
    {
        std::cout << "main: unhandled error: '" << e.what() << "'" << std::endl;
    }

    try
    {
        std11::expected<int> ok(0);

        ok.apply( reader() )
          .apply( printer() )
//          .handle( solver() )
          .handle( thrower() )
          ;

        std11::expected<int> ee( std11::exceptional, std::make_exception_ptr( std::runtime_error("not a value") ) );
        ee.apply( printer() );
    }
    catch ( std::exception const & e )
    {
        std::cout << "main: unhandled error: '" << e.what() << "'" << std::endl;
    }

#if 0
    try
    {
        std11::expected<std::string> ss( "12" );

        ss.apply( s_printer() )
          .handle( i_doubler() )
          .apply( printer() )
          .handle( i_doubler() )
          .apply( printer() )
          ;

        std11::expected<int> vv( 77 );

        std::cout << "vv.value(): " << vv.value() << std::endl;
        std::cout << "*vv: " << *vv << std::endl;
        print_s( vv );
        vv.apply( printer() )
          .apply( printer() )
          ;
        print( vv );

        std11::expected<int> ee( std11::exceptional, std::make_exception_ptr( std::runtime_error("not a value") ) );

        vv.swap( ee );
        ee.apply( printer() );

        vv.swap( ee );
        print_s( ee );

        std::cout << "solver, vv:" << std::endl;
        vv.apply( printer() )
          .handle( i_doubler() )
          .handle( solver() )
          .apply( printer() )
          ;

        std::cout << "solver, ee:" << std::endl;
        ee.handle( solver() )
          .apply( printer() )
          ;

//int x = ee.value();
        print( ee );
        ee.apply( printer() );
        std::cout << "ee.value(): " << ee.value() << std::endl;




//        foo( 33 )
//        .then  ( print )
//        .then  ( print )
//        .then  ( foo( 55 ) )
//        .then  ( print )
//        .then  ( foo( 77 ) )
//        .then  ( print )
//        .handle( warn )
//        .then  ( blow )
//        .handle( fail )
//        .then  ( foo( 11 ) )
//        .then  ( foo( 22 ) )
//        .then  ( print )
//        .handle( warn )
        ;
    }
    catch ( std::exception const & e )
    {
        std::cout << "main: unhandled error: '" << e.what() << "'" << std::endl;
    }
#endif

    return 0; //VC6
}

// cl -nologo -W3 -EHsc -Ideps test_expected.cpp && test_expected
// cl -nologo -W3 -EHsc -DSTD11_USE_AUTO_PTR test_expected.cpp && test_expected

// g++ -Wall -Wextra -std=c++11 -o test_expected.exe test_expected.cpp && test_expected
