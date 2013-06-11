//
// expected - variations on Expected<T> by Andrei Alexandrescu.
//
// Written in 2013 by Martin Moene <martin.moene@gmail.com>
//

#include "expected.h"
#include <cassert>

// Use example(callee)
#if 0

Expected<int> parseInt( conststd::string & s )
{
    int result;
    ...

    if( nonDigit )
    {
        return Expected<int>::fromException( std::invalid_argument( "not a number" ) );
    }

    ...

    if( tooManyDigits )
    {
        return Expected<int>::fromException( std::out_of_range( "overflow" ) );
    } ...

    return result;
}

#endif


// Use example(caller)
#if 0

//Caller

strings = readline();
auto x = parseInt( s ).get();   //throw on error
auto y = parseInt( s );         //won’t throw

if( !y.valid() )
{
    //handle locally

    if( y.hasException<std::invalid_argument>() )
    {
        // nodigits
        ...
    }

    y.get(); //just"re"throw
}
#endif

// ---------------------------------------------------------------------------

#include <iostream>

Expected<int> foo()
{
   return 33;
}

Expected<int> bar()
{
   return Expected<int>::fromException( std::invalid_argument( "not a number" ) );
}

int foobar()
{
    throw std::runtime_error( "exception in fromCode()" );
}

int main()
{
    try
    {
        std::cout << "foo().get(): " << foo().get() << std::endl;
        std::cout << "bar().get(): " << bar().get() << std::endl;

        Expected<int>::fromCode( foobar ).get();
    }
    catch ( std::invalid_argument const & e )
    {
        std::cout << "Invalid argument: " << e.what() << std::endl;
    }
    catch ( std::exception const & e )
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
    catch (...)
    {
        assert( false );
    }

    return 0; // VC6
}

// cl -nologo -W3 -EHsc -GR -I.. -I../deps test_expected.cpp  && test_expected.exe
// g++ -Wall -Wextra -std=c++11 -o test_expected.exe test_expected.cpp  && test_expected.exe
