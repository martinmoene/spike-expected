// Copyright (C) 2013 Martin Moene <martin.moene@gmail.com>
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "expected.h"
#include <cassert>
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
}

// cl -nologo -W3 -EHsc -GR test_expected.cpp  && test_expected.exe
// g++ -Wall -Wextra -std=c++11 -o test_expected.exe test_expected.cpp  && test_expected.exe
