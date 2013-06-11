//
// expected - variations on Expected<T> by Andrei Alexandrescu.
//
// Anto Nonco. Extending expected<T> to deal with references. 27 May 2013.
// http://anto-nonco.blogspot.nl/2013/03/extending-expected-to-deal-with.html
//

#include "expected-nonco.h"

#include <cassert>
#include <iostream>

expected<int> foo()
{
   return 33;
}

expected<int> bar()
{
   return expected<int>::from_exception( std::invalid_argument( "not a number" ) );
}

int foobar()
{
    throw std::runtime_error( "exception in fromCode()" );
}

int main()
{
    try
    {
        std::cout << "*foo(): " << *foo() << std::endl;
        std::cout << "*bar(): " << *bar() << std::endl;

        *expected<int>::from_code( foobar );
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

// cl -nologo -Wall -dep expected-nonco.cpp  && expected-nonco.exe
// g++ -Wall -Wextra -std=c++11 -o expected-nonco.exe expected-nonco.cpp  && expected-nonco.exe
